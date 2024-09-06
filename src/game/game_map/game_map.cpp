#include"pch.h"
#include"game_map.h"

// unstringify an int array
template<typename T>
void StringToIntArr(std::string& str_in, std::vector<T>& arr_out) {
	// remove unneccery symbols
	str_in.erase(std::remove(str_in.begin(), str_in.end(), ' '), str_in.cend());
	str_in.erase(std::remove(str_in.begin(), str_in.end(), '\n'), str_in.cend());
	assert(str_in[str_in.size() - 1] != ',', "extra comma at the end of array!");
	// add extra comma at the end so we can read the last number
	str_in += ",";
	// split by comma
	// each int will be picked out of the string by finding the comma
	//
	// num_beg - where the integer starts as a string
	// num_end - where the integer ends as a string
	// NOTE: num_beg & num_end point to index of comma
	// NOTE: the string below is formated as it woud be when being passed into the for loop
	//		   +-------------- num_beg
	//		   |   +---------- num_end
	//         v   v
	// "128,255,176,6,48,"	 
	for (size_t num_beg = 0, num_end = str_in.find(','); num_end != std::string::npos; num_beg = num_end + 1, num_end = str_in.find(',', num_beg)) {
		arr_out.push_back(std::stoi(str_in.substr(num_beg, num_end - num_beg)));
	}
}

GameMap::GameMap(const char* map_filename, const char* texture_filename, const char* map_data_filename, Camera& camera, size_t texture_size) : m_tiles(map_filename), m_texture_atlas(texture_filename), m_texture_size(texture_size) {
	// read map data from file
	// open the file
	std::ifstream in(map_data_filename);
	assert(!in.fail(), "file doesnt exist!");
	// read file to string
	std::string f_contents((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
	// removing all of the comments from the file (marked as "//" going until the line ending)  
	for (size_t com_beg = f_contents.find("//"), com_end = f_contents.find('\n', com_beg); com_beg != std::string::npos; com_beg = f_contents.find("//", com_beg), com_end = f_contents.find('\n', com_beg)) {
		std::string comment = f_contents.substr(com_beg, com_end - com_beg);
		f_contents.erase(com_beg, com_end - com_beg);
	}
	// remove unwanted characters
	f_contents.erase(std::remove(f_contents.begin(), f_contents.end(), ' '), f_contents.cend());
	f_contents.erase(std::remove(f_contents.begin(), f_contents.end(), '\n'), f_contents.cend());
	f_contents.erase(std::remove(f_contents.begin(), f_contents.end(), '\t'), f_contents.cend());
	// output dictionary of string of string
	// about std::unordered_map: https://www.geeksforgeeks.org/unordered_map-in-cpp-stl/
	std::unordered_map<std::string, std::string> dictionary;
	// loop trough the keys and values in file and pass them into the dictionary
	for (size_t word_beg = 0, word_end = f_contents.find('{'); word_end != std::string::npos; word_beg = word_end + 1, word_end = f_contents.find('{', word_beg)) {
		// first get the key
		// NOTE: the key will be always before the value
		std::string key_word = f_contents.substr(word_beg, word_end - word_beg);
		// then the value
		// NOTE: value is stored between the brackets
		word_beg = word_end + 1; // start from the opening bracket
		word_end = f_contents.find('}', word_beg);
		std::string value_word = f_contents.substr(word_beg, word_end - word_beg);
		// add to dictionary
		dictionary[key_word] = value_word;
	}
	// assigning values to variables from file
	// get tile colors
	// check if file contains needed keys
	assert(!(dictionary.find("empty_tile_color") == dictionary.end()), "file doesnt contain 'empty_tile_color' key!");
	assert(!(dictionary.find("tile_color_list") == dictionary.end()), "file doesnt contain 'tile_color_list' key!");
	// set m_empty_tile_color value from read file
	StringToIntArr<uint8_t>(dictionary["empty_tile_color"], m_empty_tile_color);
	assert(m_empty_tile_color.size() % 3 == 0, "'m_empty_tile_color' not alighned with the RGB channels"); // check if m_texture_ids is in multiple of 3 (RGB channels)
	// set m_texture_ids values from read file
	StringToIntArr<uint8_t>(dictionary["tile_color_list"], m_texture_ids);
	assert(m_texture_ids.size() % 3 == 0, "'tile_color_list' not alighned with the RGB channels"); // check if m_texture_ids is in multiple of 3 (RGB channels)
	// get player position & angle
	// if player tile color is specified set camera position
	if (!(dictionary.find("player_tile_color") == dictionary.end())) {
		// get value from dictionary
		std::vector<int> player_tile_color;
		StringToIntArr<int>(dictionary["player_tile_color"], player_tile_color);
		// loop trough the entire map texture and get position of the player tile / pixel
		for (size_t y = 0; y < m_tiles.GetHeight(); y++) {
			for (size_t x = 0; x < m_tiles.GetWidth(); x++) {
				if (m_tiles.Get(x, y, 0) == player_tile_color[0] && m_tiles.Get(x, y, 1) == player_tile_color[1] && m_tiles.Get(x, y, 2) == player_tile_color[2]) {
					// set camera position to the player tile if found
					camera.SetPosition(vec2((float)x + 0.5f, (float)y + 0.5f));
					// set tile to empty when found so it is not to be taken as a wall
					m_tiles.Set(m_empty_tile_color[0], x, y, 0);
					m_tiles.Set(m_empty_tile_color[1], x, y, 1);
					m_tiles.Set(m_empty_tile_color[2], x, y, 2);
				}
			}
		}
	}
	// set camera angle from dictionary if specified
	if (!(dictionary.find("player_angle") == dictionary.end())) {
		// NOTE: input angle is in degrees
		camera.SetAngle(Math::toRad((float)std::stoi(dictionary["player_angle"])));
	}
}

size_t GameMap::GetTileID(vec2 point) {
    // check if 2d point lies on a full tile with O(1) speed!!
	// margin of error
	const float margin = 0.01f;
	// get tile position by flooring the coordinates
	vec2 tile = vec2(floor(point.x), floor(point.y));
	// check if not out of bounds
	if ((size_t)tile.x < 1 || (size_t)tile.x > m_tiles.GetWidth() || (size_t)tile.y < 1 || (size_t)tile.y > m_tiles.GetHeight()) return 0;
	// check tile
	if (CheckIfTileNotEmpty((size_t)tile.x, (size_t)tile.y)) return TilePositionToID((size_t)tile.x, (size_t)tile.y);
	// also check at positive X & Y edges
	// because floor(1.0) returns 1.0 not 0.0 
	// so the it floors to the tile next to the desiered tile
	if (point.x - tile.x < margin) {
		if (CheckIfTileNotEmpty((size_t)tile.x - 1, (size_t)tile.y)) return TilePositionToID((size_t)tile.x - 1, (size_t)tile.y);
	}
	if (point.y - tile.y < margin) {
		if (CheckIfTileNotEmpty((size_t)tile.x, (size_t)tile.y - 1)) return TilePositionToID((size_t)tile.x, (size_t)tile.y - 1);
	}
	// return 0 if not found
	// NOTE: 0 also corresponds to the first tile in the map wich will be never seen.
    return 0;
 }

bool GameMap::CheckIfTileNotEmpty(size_t x, size_t y) {
	if (m_tiles.Get(x, y, 0) != m_empty_tile_color[0] || m_tiles.Get(x, y, 1) != m_empty_tile_color[1] || m_tiles.Get(x, y, 2) != m_empty_tile_color[2]) return true;
	return false;
}

size_t GameMap::GetTileTextureID(size_t id) {
	// get X & Y coordinates of texture from id
    size_t x = id % m_tiles.GetWidth();
    size_t y = id / m_tiles.GetWidth();
	// search trough the texture_id list and check for match
    for (size_t i = 0; i < m_texture_ids.size(); i += 3) {
        if (
        m_tiles.Get(x, y, 0) == m_texture_ids[i] && 
        m_tiles.Get(x, y, 1) == m_texture_ids[i + 1] && 
        m_tiles.Get(x, y, 2) == m_texture_ids[i + 2]) {
			// return texture id
			// textur id is just the index of texture in the texture atlas
            return i / 3; 
        }
    }
    //printf("texture not found id: %llu, x: %llu, y: %llu\n", id, x, y);
	printf("texture not found id: %llu, rgb: %d, %d, %d\n", id, m_tiles.Get(x, y, 0), m_tiles.Get(x, y, 1), m_tiles.Get(x, y, 2));
    // return default texture if not found
	return 0;
}

uint8_t GameMap::GetTextureColor(size_t texture_id, float x, float y) {
	// first we get the texture position in the texture_map with texture_id.
	// then we get the texture color relative to the texture bit in the map.
	size_t uv_x = (texture_id % (m_texture_atlas.GetWidth() / m_texture_size)) * m_texture_size + (size_t)(x * (float)m_texture_size);
	// same for y
	size_t uv_y = (texture_id / (m_texture_atlas.GetWidth() / m_texture_size)) * m_texture_size + (size_t)(y * (float)m_texture_size);
	// get color from texture
	uint8_t color = m_texture_atlas.Get(uv_x, uv_y);
	return color;
}


Texture2D& GameMap::GetMapTexture() {
	return m_tiles;
}

Texture2D& GameMap::GetAtlasTexture() {
	return m_texture_atlas;
}

size_t GameMap::TilePositionToID(size_t x, size_t y) {
	return x + y * m_tiles.GetWidth();
}