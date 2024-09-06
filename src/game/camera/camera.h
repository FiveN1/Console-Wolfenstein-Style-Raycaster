#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include"pch.h"

/*
* Camera Class
*
*/
class Camera {
public:
    // Setup defult camera values
    Camera();
    // Setup custom camera values
    Camera(vec2 position, float angle);
    // Get user input
    void GetInput();
    // Set angle of camera
    void SetAngle(float angle);
    // Get camera angle
    float GetAngle();
    // Set camera position
    void SetPosition(vec2 position);
    // get camera position
    vec2 GetPosition();
    // Camera data
private:
    // camera position in the game space
    vec2 position;
    // 2D angle of the camera
    float angle;
    // Speed data
    const float m_speed;
    const float m_max_speed;
};
#endif CAMERA_CLASS_H