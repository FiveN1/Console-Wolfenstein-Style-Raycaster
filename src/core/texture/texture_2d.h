#ifndef TEXTURE_2D_CLASS_H
#define TEXTURE_2D_CLASS_H

#include"pch.h"

// 2D buffer that stores values of type u8 aka char
class Texture2D {
public:
    // Allocate memory for the buffer
    Texture2D(size_t width, size_t height, uint8_t color_channels = 1);
    // Load texture
    Texture2D(const char* filename);
    // Deallocate memory for the buffer
    ~Texture2D();
    // Set value in buffer
    void Set(uint8_t value, size_t x, size_t y, size_t channel = 0);
    // Get value from buffer
    uint8_t Get(size_t x, size_t y, size_t channel = 0);
    // Set whole buffer to one value
    void SetAll(uint8_t value);
    // Get buffer width
    size_t GetWidth();
    // Get buffer height
    size_t GetHeight();
    // Get size of buffer value type
    size_t GetTypeSize();
    // Buffer variables
private:
    uint8_t* m_data;    // Pointer to block of data
    size_t m_width;     // Width of buffer
    size_t m_height;    // Height of buffer
    uint8_t m_color_channels;
};
#endif TEXTURE_2D_CLASS_H