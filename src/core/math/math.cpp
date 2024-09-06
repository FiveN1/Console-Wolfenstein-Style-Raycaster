#include"pch.h"
#include"math.h"

float Math::toRad(float degree) {
	return degree * (PI / 180.0f);
}

float Math::toDeg(float radiant) {
	return radiant * (180.0f / PI);
}