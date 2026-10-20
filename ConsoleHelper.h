#pragma once

class ConsoleHelper
{
public:
    static void clear();
    static void hideCursor();
    static void showCursor();

    static int width();
    static int height();

    static void setColor(int color);
    static void resetColor();

    static void moveCursor(int x, int y);
    static void sleepMilliseconds(int milliseconds);
};