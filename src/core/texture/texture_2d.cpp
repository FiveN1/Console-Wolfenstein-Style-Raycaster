#include"pch.h"
#include"texture_2d.h"
#include"stb/stb_image_imp.h"

Texture2D::Texture2D(size_t width, size_t height, uint8_t color_channels) : m_width(width), m_height(height), m_color_channels(color_channels) {
    m_data = (uint8_t*)malloc(sizeof(uint8_t) * m_width * m_height * m_color_channels); // allocate memory
    SetAll(0); // set whole buffer to one value
}

Texture2D::Texture2D(const char* filename) : m_width(0), m_height(0), m_color_channels(1) {
    int width, height, channels;
    m_data = (uint8_t*)stb::load(filename, &width, &height, &channels, 0);
    assert(m_data != nullptr, "Cant load texture from file!");
    m_width = width;
    m_height = height;
    m_color_channels = channels;
}

Texture2D::~Texture2D() {
    // Free memory
    free(m_data);
    m_data = nullptr;
}

void Texture2D::Set(uint8_t value, size_t x, size_t y, size_t channel) {
    if (channel >= m_color_channels) {
        printf("ERROR: Texture out of channels\n");
        return;
    }
    m_data[(x + y*m_width) * m_color_channels + channel] = value;
}

uint8_t Texture2D::Get(size_t x, size_t y, size_t channel) {
    if (channel >= m_color_channels) {
        printf("ERROR: Texture out of channels\n");
        return 0;
    }
    return m_data[(x + y*m_width) * m_color_channels + channel];
}

void Texture2D::SetAll(uint8_t value) {
    memset(m_data, value, sizeof(uint8_t) * m_width * m_height);
}

size_t Texture2D::GetWidth() {
    return m_width;
}

size_t Texture2D::GetHeight() {
    return m_height;
}

size_t Texture2D::GetTypeSize() {
    return sizeof(uint8_t);
}