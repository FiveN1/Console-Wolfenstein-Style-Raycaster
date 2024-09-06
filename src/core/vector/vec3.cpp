#include"pch.h"
#include"vec3.h"

vec3::vec3(): x(0.0), y(0.0), z(0.0) {}

vec3::vec3(float _x, float _y, float _z): x(_x), y(_y), z(_z) {}

vec3::vec3(vec3& ref) : x(ref.x), y(ref.y), z(ref.z) {}

bool vec3::operator==(vec3 vec) {
    if (x == vec.x && y == vec.y && z == vec.z) 
        return true;
    return false;
}