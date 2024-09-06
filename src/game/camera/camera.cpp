#include"pch.h"
#include"camera.h"


Camera::Camera() : position(0.0f, 0.0f), angle(0.0f), m_speed(1.0f), m_max_speed(2.0f) {}

Camera::Camera(vec2 _position, float _angle) : position(_position), angle(_angle), m_speed(2.0f), m_max_speed(2.0f) {}

void Camera::GetInput() {
    vec2 keys = {
		(float)(bool)(GetKeyState('A') & 0x8000) - (float)(bool)(GetKeyState('D') & 0x8000),
		(float)(bool)(GetKeyState('S') & 0x8000) - (float)(bool)(GetKeyState('W') & 0x8000)
    };

	vec2 direction = vec2(sin(angle - 1.57079f) * keys.x -sin(angle) * keys.y, cos(angle) * keys.y - cos(angle - 1.57079f) * keys.x);
    vec2 motion = direction * 0.002f * m_speed;

	position += motion;

    // First click
    static bool firstclick = false;
	if (GetKeyState(VK_RBUTTON) & 0x8000 && !firstclick) {
		firstclick = true;
		SetCursorPos(500, 500);
	}
	if (!(GetKeyState(VK_RBUTTON) & 0x8000) && firstclick) {
		firstclick = false;
	}

	if (GetKeyState(VK_RBUTTON) & 0x8000) { // VK_RBUTTON
		// Get mouse position
		POINT m;
		GetCursorPos(&m);
		// Get mouse movement
		float rotX = 0.002f * ((float)m.x - 500.0f);
		angle += rotX;
		// reset mouse position
		SetCursorPos(500, 500);
	}
}


void Camera::SetAngle(float _angle) {
    angle = _angle;
}


float Camera::GetAngle() {
    return angle;
}

void Camera::SetPosition(vec2 _position) {
    position = _position;
}

vec2 Camera::GetPosition() {
    return position;
}