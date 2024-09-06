#include"pch.h"
#include"console.h"

Console* Console::sp_instance = nullptr;

Console::Console() {
    // Set static instance
    assert(!sp_instance, "Console singleton already exists!");
    sp_instance = this;
    // Get handle
    m_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO console_info;
    // Get default attrib
    GetConsoleScreenBufferInfo(m_handle, &console_info);
    m_default_attrib = console_info.wAttributes;
    // Get number of rows & columns
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(m_handle, &csbi);
    m_columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    m_rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

Console::~Console() {
    CloseHandle(m_handle);
}

void Console::Write(const char* data, COORD coord) {
    DWORD characters = 0;
    WriteConsoleOutputCharacterA(m_handle, data, (DWORD)strlen(data), coord, &characters);
}

void Console::WriteChar(char data, COORD coord) {
    DWORD characters = 0;
    WriteConsoleOutputCharacterA(m_handle, &data, 1, coord, &characters);
}

void Console::WriteColor(const char* data, COORD coord, WORD color) {
    DWORD characters = 0;
    std::vector<WORD> attributes((DWORD)strlen(data), color); // Can optimize from vector to something else?
    WriteConsoleOutputAttribute(m_handle, &attributes[0], (DWORD)attributes.size(), coord, &characters);
    WriteConsoleOutputCharacterA(m_handle, data, (DWORD)strlen(data), coord, &characters);
}

bool Console::Clear() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(m_handle, &csbi)) return false;
    CHAR_INFO fi = { ' ', csbi.wAttributes };
    csbi.srWindow.Left = 0;
    csbi.srWindow.Top = 0;
    csbi.srWindow.Right = csbi.dwSize.X - 1;
    csbi.srWindow.Bottom = csbi.dwSize.Y - 1;
    SMALL_RECT* lpScrollRectangle = &csbi.srWindow;
    SetConsoleCursorPosition(m_handle, {0, 0}); // reset cursor position
    return ScrollConsoleScreenBufferW(m_handle, lpScrollRectangle, 0, csbi.dwSize, &fi);
}

void Console::SetColor(WORD color_code) {
    SetConsoleTextAttribute(m_handle, color_code);
}

void Console::SetColorDefault() {
    SetConsoleTextAttribute(m_handle, m_default_attrib);
}

INPUT_RECORD Console::GetEvent() {
    // https://stackoverflow.com/questions/41212646/get-key-press-in-windows-console
    // https://stackoverflow.com/questions/46658472/non-blocking-readconsoleinput
    DWORD cc;
    INPUT_RECORD irec;
    ReadConsoleInput(m_handle, &irec, 1, &cc);
    if (!ReadConsoleInput(m_handle, &irec, 1, &cc)) {
        printf("error: cant read console input. error code: %hu\n", GetLastError());
    }
    //printf("Get event\n");
    //printf("key: %c\n", ((KEY_EVENT_RECORD&)irec.Event).uChar.AsciiChar);
    printf("event type: %hu\n", irec.EventType);
    return irec;
}

void Console::SetTitle(const char* title) {
    SetConsoleTitleA(title);
}

void Console::SetFont(const wchar_t* font_name, SHORT font_size) {
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    cfi.nFont = 0;
    cfi.dwFontSize.X = 0;
    cfi.dwFontSize.Y = font_size;
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    wcscpy_s(cfi.FaceName, font_name); //L"Consolas"
    SetCurrentConsoleFontEx(m_handle, FALSE, &cfi);
}

void Console::SetDisplayMode(DWORD mode) {
    SetConsoleDisplayMode(m_handle, mode, 0);
}

size_t Console::GetRows() {
    return m_rows;
}

size_t Console::GetColumns() {
    return m_columns;
}

void Console::DrawTexture(Texture2D& texture, const char* charmap) {
    // create a row buffer so we dont draw character by character but the whole row at once, wich is faster
    std::string row_buffer;
    row_buffer.resize(texture.GetWidth());
    // the loop
	for (size_t y = 0; y < texture.GetHeight(); y++) {
		for (size_t x = 0; x < texture.GetWidth(); x++) {
            // first draw to the row buffer
			row_buffer[x] = charmap[(int)((1.0 - ((float)texture.Get(x, y) / 255.0f)) * (strlen(charmap) - 1))];
		}
        // then draw the whole row buffer to console
		COORD coord = {0, (SHORT)y};
		this->Write(row_buffer.c_str(), coord);
	}
}