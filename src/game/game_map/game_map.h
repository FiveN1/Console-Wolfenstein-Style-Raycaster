#ifndef GAME_MAP_CLASS_H
#define GAME_MAP_CLASS_H

#include"pch.h"

#include"core/texture/texture_2d.h"
// camera is part of game map
#include"game/camera/camera.h"

/*
* Game Map Class
* - holds all the data needed for a playable game map
*
*/
class GameMap {
public:
    // load the game map from an image!
    GameMap(const char* map_filename, const char* texture_filename, const char* map_data_filename, Camera& camera, size_t texture_size = 16);
    // id is just the one-dimensional index of two-dimensional array
    size_t GetTileID(vec2 point);
    // check if tile contains a wall at the index
    bool CheckIfTileNotEmpty(size_t x, size_t y);
    // get tile texture at location
    // NOTE: location of the textur in the atlas is stored as one integer (id)
    size_t GetTileTextureID(size_t id);
    // get texture color from texture map map 
    // NOTE: X & Y must range from (0.0) to (1.0)
    uint8_t GetTextureColor(size_t texture_id, float x, float y);
    // Texture manipulation
public:
    // Get map data
    Texture2D& GetMapTexture();
    // Get texture atlas data
    Texture2D& GetAtlasTexture();
    // private game map functions
private:
    // get 1-dimensional index from 2-dimensional position
    size_t TilePositionToID(size_t x, size_t y);
    // private game map variables
private:
    // actual data of the map tiles
    Texture2D m_tiles;
    // texture atlas storing all of the textures used in the map
    Texture2D m_texture_atlas;
    // size (width & height) of one texture in the atlas
    size_t m_texture_size;
    // texture id array for setting tile data to the coresponding textur in the atlas
    std::vector<uint8_t> m_texture_ids;
    std::vector<uint8_t> m_empty_tile_color;
};
#endif GAME_MAP_CLASS_H