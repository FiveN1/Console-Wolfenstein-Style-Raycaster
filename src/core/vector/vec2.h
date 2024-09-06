#ifndef VEC2_CLASS_H
#define VEC2_CLASS_H

class vec2 {
public:

    vec2();

    vec2(float x, float y);

    vec2(vec2& ref);

public:

    float length();

public:

    void operator=(vec2 vec);

    vec2 operator*(float value);

    vec2 operator*(vec2 vec);

    vec2 operator/(float value);

    vec2 operator/(vec2 vec);

    vec2 operator+(vec2 vec);

    vec2 operator-(vec2 vec);

    void operator*=(float value);

    void operator*=(vec2 vec);

    void operator/=(float value);

    void operator/=(vec2 vec);

    void operator+=(vec2 vec);

    void operator-=(vec2 vec);

public:
    float x;
    float y;
};

vec2 normalize(vec2 vector);

#endif VEC2_CLASS_H