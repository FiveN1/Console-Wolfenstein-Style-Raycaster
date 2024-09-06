#ifndef RAYCAST_SYSTEM_H
#define RAYCAST_SYSTEM_H

#include"pch.h"

#include"core/texture/texture_2d.h"

#include"../game_map/game_map.h"

namespace Raycaster {

     enum AntiAliasingTypes {
        NO_AA = 0,
        TWO_X_AA,
        FOUR_X_AA,
        EIGHT_X_AA,
    };

    void Render(Texture2D& viewport, GameMap& map, vec2 camera_position, float camera_angle);

    void RenderMipMap(Texture2D& viewport, GameMap& map, vec2 camera_position);

    void RenderFloor(Texture2D& viewport);

    char ValueToAscii(float value, const char* charmap);
    
}
#endif RAYCAST_SYSTEM_H