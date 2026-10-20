#include "ConsoleHelper.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <windows.h>

void ConsoleHelper::clear()
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO info{};
    if (!GetConsoleScreenBufferInfo(h, &info)) return;

    DWORD cells = static_cast<DWORD>(info.dwSize.X) * info.dwSize.Y;
    DWORD written = 0;
    COORD home{0, 0};
    FillConsoleOutputCharacterA(h, ' ', cells, home, &written);
    FillConsoleOutputAttribute(h, info.wAttributes, cells, home, &written);
    SetConsoleCursorPosition(h, home);
}

void ConsoleHelper::setColor(int color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                            static_cast<WORD>(color));
}

void ConsoleHelper::resetColor()
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}

void ConsoleHelper::moveCursor(int x, int y)
{
    COORD position{static_cast<SHORT>(x), static_cast<SHORT>(y)};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
}

void ConsoleHelper::hideCursor()
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info{};
    if (GetConsoleCursorInfo(h, &info)) {
        info.bVisible = FALSE;
        SetConsoleCursorInfo(h, &info);
    }
}

void ConsoleHelper::showCursor()
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info{};
    if (GetConsoleCursorInfo(h, &info)) {
        info.bVisible = TRUE;
        SetConsoleCursorInfo(h, &info);
    }
}

int ConsoleHelper::width()
{
    CONSOLE_SCREEN_BUFFER_INFO info{};
    if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info))
        return 80;
    return info.srWindow.Right - info.srWindow.Left + 1;
}

int ConsoleHelper::height()
{
    CONSOLE_SCREEN_BUFFER_INFO info{};
    if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info))
        return 25;
    return info.srWindow.Bottom - info.srWindow.Top + 1;
}

void ConsoleHelper::sleepMilliseconds(int milliseconds)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}
