#ifndef VEC3_CLASS_H
#define VEC3_CLASS_H

class vec3 {
public:

    vec3();

    vec3(float x, float y, float z);

    vec3(vec3& ref);

public:

    bool operator==(vec3 vec);

public:
    float x;
    float y;
    float z;

};

#endif VEC3_CLASS_H