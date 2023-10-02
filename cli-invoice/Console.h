#pragma once
#include <iostream>
#include <windows.h>


inline void set_console_text_color(WORD color) {
	const HANDLE hConsole = GetStdHandle(STD_ERROR_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}
inline void error_message(const std::string& message)
{
	set_console_text_color(FOREGROUND_RED | FOREGROUND_INTENSITY); // red
	std::cerr << message << "\n";
	set_console_text_color(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // white
}
inline void warning_message(const std::string& message)
{
	set_console_text_color(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY); // yellow
	std::cerr << message << "\n";
	set_console_text_color(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // white
}
inline void success_message(const std::string& message)
{
	set_console_text_color(FOREGROUND_GREEN | FOREGROUND_INTENSITY); // green
	std::cout << message << "\n";
	set_console_text_color(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // white
}

inline void info_message(const std::string& message)
{
	set_console_text_color(FOREGROUND_BLUE | FOREGROUND_INTENSITY); // blue
	std::cout << message << "\n";
	set_console_text_color(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // white
}