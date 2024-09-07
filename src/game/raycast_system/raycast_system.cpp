#include"pch.h"
#include"raycast_system.h"

struct RayData {
	float length;
	float tan_intersection;
	size_t texture_id;
};

RayData CastRay(GameMap& map, float ray_angle, vec2 camera_position);

uint8_t GetColor(GameMap& map, size_t texture_id, float x, float y, float screen_height, float column_height, uint8_t aa_samples = 4);

float GetFogAmount(float distance, float fog_begin) {
	// the fog will begin once a certain distance has been reached
	if (distance < fog_begin) return 1.0;
	return 1.0f / (distance - fog_begin + 1.0f);
}

void Raycaster::Render(Texture2D& viewport, GameMap& map, vec2 camera_position, float camera_angle) {
	// angle of field of view
	float fov = Math::toRad(85.0);
	/* calculation of z_dis based on fov and screen width
	+------- angle1 = 90 - angle3
    v
	+----+   +
    \   .| <-|----- angle2 = 90
     \   |   |
      \  |   +----- z_dis = width/2 * tan(angle1);
       \ |   |
        \| <-|----- angle3 = fov/2
         V   +   
	*/
	float z_dis = ((float)viewport.GetWidth() * 0.5f) * tan(PI * 0.5f - fov * 0.5f);

	// iterating over every x coordinate
	for (size_t x = 0; x < viewport.GetWidth(); x++) {
        /* calculation of ray angle
		     +----------- x position
             v
        +----+----+ <---- screen
        \    |    / .
         \   |   /  .
          \  |../ <-.---- angle
           \ | /    .
            \|/     .
             V      . <-- z_dis = distance from CAMERA to SCREEN
           camera
        */
	    // calculate the angle of the ray based on the screen X position
		float ray_angle = camera_angle + atan(((float)x - (float)viewport.GetWidth() * 0.5f) / z_dis);
		// Raycast until the ray hits a wall and then get the length of the ray
		// Check the CastRay() function if you want to find out how that works!
		RayData ray = CastRay(map, ray_angle, camera_position);
		// distance from camera to wall. (just the length of the ray)
		float distance_to_wall = ray.length * cos(ray_angle - camera_angle); // * cos() to remove the fish-eye effect
		// tile edge intersection from begining of edge to end (0.0 to 1.0)
		// used for getting the correct color coordinate from texture
		float axis_inter = ray.tan_intersection;
		// index of texture in the texture atlas
		size_t texture_id = ray.texture_id;

		/*
		+-- draw one column at the time based on the
		|   calculated distance from the wall
		v
		# # | | | | | | | | | | # #
		# # # # | | | | | # # # # #
		# # # # # # # # # # # # # # _ <- Height / 2
		# # # # # # # # # # # # # #
		# # # # | | | | | # # # # #
		# # | | | | | | | | | # # #
		A           A
		|           +-- distance_to_wall = 10.0
		+-------------- distance_to_wall = 0.5
		*/
		// calculate column height based on distance_to_wall
		float column_height = (float)viewport.GetHeight() * 0.5f / distance_to_wall;
		// simple fog effect
		float fog = GetFogAmount(distance_to_wall, 2.0f);
		// to not draw out of screen if too close to wall
		if (column_height > (float)viewport.GetHeight()) column_height = (float)viewport.GetHeight();
		// column height cap is 1 so it doesnt mess with the anit-aliasing
		if (column_height < 1.0f) column_height = 1.0f;
		// draw column to the viewport
		// NOTE: column is drawn such that the middle of the column is also in the middle of the screen
		for (size_t y = (size_t)((float)viewport.GetHeight() * 0.5f - column_height * 0.5f); y < (size_t)((float)viewport.GetHeight() * 0.5f + column_height * 0.5f); y++) {
			// final color is just the texture color with the fog effect
			uint8_t color = (uint8_t)((float)GetColor(map, texture_id, axis_inter, (float)y, (float)viewport.GetHeight(), column_height, 8) * fog);
			// set screen pixel
			viewport.Set(color, x, y);
		}
	}
}

RayData CastRay(GameMap& map, float ray_angle, vec2 camera_position) {
	/* ray passing trough grid by steps
	The Process:
	- (1) calculate the nearest X & Y intersection of the ray.
		  then calculate the ray step vector wich is the diffrence between current intersection point and next 
		  (each different for X & Y variant)
	- (2) Iterate over every ray intersection point (X or Y) until the ray hits a wall

	image representation of the process below. vvv

			step.x
	         v  v
	+--------+--O-----+	
	|        | .      |  
	|        N        | < step.y
	|       .|        |  
	+------N-+--------+
	|    .   |        |
	|   .    |        |
	|  C     |        |
	+--------+--------+

	C - Camera
	O - Ray-Edge intersection
	N - Nearest intersection (one on X (ray_x) the other on Y (ray_y))
	*/

	// (1) //

	// camera coordinates relative to the tile that the camera stands on
	vec2 cam_floor = vec2(floor(camera_position.x), floor(camera_position.y));
	// camera position relative to the tile that it stands on...
	// so it only can be from 0.0 to 1.0 (aka the tile size)
	vec2 ray_position = camera_position - cam_floor;
	// so as you can imagine from the image below, tan() is the perfect function for calculating the angle x edge intersection.
	// tan(angle) returns the Y of the intersection if we assume that X = 1.0. if not then we can scale the output accordingly.
	// if we want to get the X of the intersection then we can inverse it. (1 / tan(angle))
	// but in C++ the tan function seems already inverted so its the other way around...
	/*
	 - tan of angle: 
	             . <-+ intersection
	|           /|   |
	|          / |   |
		  ...        |
	|--- /_      |   +-- tan(angle)
	|   /  --_   |   |
	|  /      |  |   |
	| /        | |   |
	|/a)       | |   |
	+------------- <-+
	  A
	  +-- angle
	*/
	// lets look at the calculation of the X coordinate of ray_x:
	// first we have: 
	// - ray_position.x 			- wich is the camera position relative to the tile
	// - tan(ray_angle) * yn 		- this is where we calculate the Y intersection... we multiply it by 'yn' so the tan function doesnt mirror it self on the other side. (above 180 deg)
	// - abs(ray_position.y - y)	- we then scale the tan output by the size of the Y coordinate. so when we move on the Y axis the X scales accordingly.
	// you can imagine moving yourself in the tile on the first image and find out why is it so...
	
	// first ray intersection on the X axis
	float y = (float)(cos(ray_angle) < 0.0f); // coordinates of the Y value for the square. can only be 0.0 or 1.0 based on the cos of angle. imagine it as a value that determines on wich side of the circle you are.
	float yn = y * 2.0f - 1.0f; // scale y so that it will be (-1) when false and (1) when true (used as variable so it is more readable! although it will be faster to calculate it probably)
	vec2 ray_x = vec2(ray_position.x - tan(ray_angle) * yn * abs(ray_position.y - y), y);
	// first ray intersection on the Y axis
	float x = (float)(sin(ray_angle) > 0.0f); // coordinates of the X value for the square. can only be 0.0 or 1.0 based on sin of angle. imagine it as a value that determines on wich side of the circle you are.
	float xn = x * 2.0f - 1.0f; // scale x so that it will be (-1) when false and (1) when true (used as variable so it is more readable! although it will be faster to calculate it probably)
	vec2 ray_y = vec2(x , ray_position.y - (1.0f / tan(ray_angle)) * xn * abs(ray_position.x - x));
	// length of ray step in one iteration
	vec2 step_x = vec2(tan(ray_angle) * -yn, yn); 			// x : tan, y : 1.0 or -1.0
	vec2 step_y = vec2(xn, (1.0f / tan(ray_angle)) * -xn); 	// x : 1.0 or -1.0, y : tan

	// (2) //

	// tile id representing the two-dimensional tile position as a number
	size_t tile_id = 0;
	// maximum number of ray iterations
	// sortof like a max render distance
	const size_t max_iterations = 32;
	// ray passing trough the grid
	// iterate until max iteration count has been reached
	// two return vales in loop because the ray can lie only on X or Y axis
	for (size_t iteration = 0; iteration < max_iterations; iteration++) {
		// check first ray intersection point located on X axis
		// if not in wall then go to the next intersection and repeat
		if (vec2(ray_x - ray_position).length() <= vec2(ray_y - ray_position).length()) {
			tile_id = map.GetTileID(ray_x + cam_floor);
			// if return value of tile index not negative then return tile hit.
			if (tile_id != 0) {
				// ray intersection where it goes along the X axis
				float x_intersection = (ray_x.x + cam_floor.x);
				return {
					vec2(ray_x - ray_position).length(), 
					x_intersection - floor(x_intersection),
					map.GetTileTextureID(tile_id)
				};
			}
			ray_x += step_x;
		// then check ray next intersection on Y axis
		} else {
			tile_id = map.GetTileID(ray_y + cam_floor);
			if (tile_id != 0) {
				// ray intersection where it goes along the Y axis
				float y_intersection = (ray_y.y + cam_floor.y);
				return {
					vec2(ray_y - ray_position).length(), 
					y_intersection - floor(y_intersection),
					map.GetTileTextureID(tile_id)
				};
			}
			ray_y += step_y;
		}
	}
	return {
		256.0f, // return some big value if max iteration count reached
		0.0,	// default intersection 
		0 		// default texture
	};
}

uint8_t GetColor(GameMap& map, size_t texture_id, float x, float y, float screen_height, float column_height, uint8_t aa_samples) {
	
	// Process:
	// (1) check if sample point is or isnot in the column.
	// (3) if yes get texture color else black.
	// (4) repeat for number of samples and get avarage.

	// avarage pixel color
	int sampled_value = 0;
	for (uint8_t aa_c = 0; aa_c < aa_samples; aa_c++) {
		// Calculate the Anti-Aliasing point in one pixel of the column
		// NOTE: 1.0f in here means the height of one pixel. (pixel height is: 1.0f)
		float sample_point = (1.0f / aa_samples) * aa_c + (1.0f / aa_samples) * 0.5f;
		// Add up to avarage color
		// Check if the point lies inside of the column
		if (y + sample_point + 1.0f < screen_height * 0.5f + column_height * 0.5f && // +1 so it will AA the bottom of the column (y cant be equal to the column height)
			y + sample_point > screen_height * 0.5f - column_height * 0.5f) {
			// scale Y so it ranges from 0.0 to 1.0
			float y_scaled = (float)(y - (size_t)(screen_height * 0.5f - column_height * 0.5f)) / (column_height);
			// get texture color from sample point
			// NOTE: sample point is also scaled by the pixel size on the texture
			// x is scaled from 0.0 to 1.0 (start of tile edge to end of tile edge)
			// y is also scaled from 0.0 to 1.0 (column begining to column end)
			// NOTE: column_height must not be smaller then 0!
			sampled_value += map.GetTextureColor(texture_id, x, y_scaled + sample_point / column_height);

		} // else add black color
		else {
			sampled_value += 0;
		}
	}
	// Return avarage color of the pixel
	return sampled_value / aa_samples;
}


void Raycaster::RenderMipMap(Texture2D& viewport, GameMap& map, vec2 camera_position) {
	// Mipmap size
	size_t width = 12*2;
	size_t height = 12;
	// Drawing the mipmap to the viewport
	for (size_t y = 0; y < height; y++) {
		for (size_t x = 0; x < width; x++) {
			// Get map coordinates relative to the camera
			size_t xx = (x - width / 2 + (size_t)(camera_position.x * 2.0f)) / 2;
			size_t yy = y - height / 2 + (size_t)camera_position.y;
			// Default color black
			uint8_t color = 0;
			if (xx >= 0 && xx < map.GetMapTexture().GetWidth() &&
			yy >= 0 && yy < map.GetMapTexture().GetHeight()) {
				// If tile not empty then draw white
				color = map.CheckIfTileNotEmpty(xx, yy) * 255;
			}
			viewport.Set(color, x, y);
		}
	}
	// Draw center of mipmap grey so we know where we are (the camera is in the centre of the mipmap)
	viewport.Set(128, width / 2, height / 2);
}


void Raycaster::RenderFloor(Texture2D& viewport) {
	for (size_t y = viewport.GetHeight() / 2; y < viewport.GetHeight(); y++) {
		for (size_t x = 0; x < viewport.GetWidth(); x++)  {
			viewport.Set(16, x, y);
		}
	}
}