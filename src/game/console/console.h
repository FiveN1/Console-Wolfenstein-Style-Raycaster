#ifndef CONSOLE_H
#define CONSOLE_H

#include"pch.h"

#include"core/texture/texture_2d.h"

// https://www.youtube.com/watch?v=UYTkjicnFOI
// https://stackoverflow.com/questions/23369503/get-size-of-terminal-window-rows-columns

// Windows console handler singleton class
class Console {
public:
    // Console class constructor
    Console();
    // Console class destructor
    ~Console();
    // Get console instance reference
    static Console& Get() { return *sp_instance; }
    // Console functionalities
public:
    // Write to console character buffer
    void Write(const char* data, COORD coord = {});
    // Write char to console character buffer
    void WriteChar(char data, COORD coord = {});
    // Write to console character buffer with color!
    void WriteColor(const char* data, COORD coord = {}, WORD color = 0x0C);
    // Clear entire conole text buffer
    bool Clear();
    // Set console color
    void SetColor(WORD color_code);
    // Set console default color
    void SetColorDefault();
    // Get input event from console
    INPUT_RECORD GetEvent();
    // Set console window title
    void SetTitle(const char* title);
    // Set console font and size
    void SetFont(const wchar_t* font_name, SHORT font_size);
    // set console window mode
    void SetDisplayMode(DWORD mode);
    // Get number of rows in console
    size_t GetRows();
    // Get number of columns in console
    size_t GetColumns();
    // More "specific" functionalities
public:
    // Draws a texture in console
    void DrawTexture(Texture2D& texture, const char* charmap);
    // Console variables
private:
    // Console handle
    HANDLE m_handle;
    // Default color attributes
    WORD m_default_attrib; 
    // Console text buffer size
    size_t m_rows;
    size_t m_columns;
    // Static console instance pointer
    static Console* sp_instance;
};

// color defines

#define CONCOL_BLACK    0x0000
#define CONCOL_BLUE     0x0001
#define CONCOL_GREEN    0x0002
#define CONCOL_AQUA     0x0003
#define CONCOL_RED      0x0004
#define CONCOL_PURPLE   0x0005
#define CONCOL_YELLOW   0x0006
#define CONCOL_WHITE    0x0007

#endif CONSOLE_H