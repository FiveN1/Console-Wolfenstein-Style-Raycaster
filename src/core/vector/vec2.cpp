#include"pch.h"
#include"vec2.h"

vec2::vec2() : x(0.0f), y (0.0f) {}

vec2::vec2(float _x, float _y) : x(_x), y(_y) {}

vec2::vec2(vec2& ref) : x(ref.x), y(ref.y) {}

float vec2::length() {
    return sqrt(x*x + y*y);
}

void vec2::operator=(vec2 vec) {
    x = vec.x;
    y = vec.y;
}

vec2 vec2::operator*(float value) {
    return {x * value, y * value};
}

vec2 vec2::operator*(vec2 vec) {
    return {x * vec.x, y * vec.y};
}

vec2 vec2::operator/(float value) {
    return {x / value, y / value};
}

vec2 vec2::operator/(vec2 vec) {
    return {x / vec.x, y / vec.y};
}

vec2 vec2::operator+(vec2 vec) {
    return {x + vec.x, y + vec.y};
}

vec2 vec2::operator-(vec2 vec) {
    return {x - vec.x, y - vec.y};
}

void vec2::operator*=(float value) {
    x *= value;
    y *= value;
}

void vec2::operator*=(vec2 vec) {
    x *= vec.x;
    y *= vec.y;
}

void vec2::operator/=(float value) {
    x /= value;
    y /= value;
}

void vec2::operator/=(vec2 vec) {
    x /= vec.x;
    y /= vec.y;
}

void vec2::operator+=(vec2 vec) {
    x += vec.x;
    y += vec.y;
}

void vec2::operator-=(vec2 vec) {
    x -= vec.x;
    y -= vec.y;
}

vec2 normalize(vec2 vector) {
    return vector / vector.length();
}