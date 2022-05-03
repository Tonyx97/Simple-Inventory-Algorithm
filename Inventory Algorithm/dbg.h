#pragma once

#include <Windows.h>
#include <Psapi.h>
#include <thread>

enum color
{
	black = 0x0,
	dark_blue = 0x1,
	dark_green = 0x2,
	dark_cyan = 0x3,
	dark_red = 0x4,
	dark_purple = 0x5,
	dark_yellow = 0x6,
	grey = 0x7,
	dark_grey = 0x8,
	blue = 0x9,
	green = 0xA,
	cyan = 0xB,
	red = 0xC,
	purple = 0xD,
	yellow = 0xE,
	white = 0xF,
};

template <typename... A>
void print(color c, std::string text, A&&... args)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), static_cast<WORD>(c));
	printf_s(text.c_str(), std::forward<A>(args)...);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), white);
}

void debug();