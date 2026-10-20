#include "Line.h"
#include "ConsoleHelper.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

namespace
{
    int randomInt(int minValue, int maxValue)
    {
        return minValue + std::rand() % (maxValue - minValue + 1);
    }
}

Line::Line(int length, int speed, bool epilepsyMode, bool randomColorPerSymbol)
    : length_(length),
    speed_(speed),
    headX_(ConsoleHelper::width()),
    lastHeadX_(ConsoleHelper::width()),
    y_(0),
    phase_(randomInt(0, 3)), 
    epilepsyMode_(epilepsyMode),
    randomColorPerSymbol_(randomColorPerSymbol),
    finished_(false),
    lineColor_(10)
{
    int maxY = ConsoleHelper::height() - 3;
    if (maxY < 1) maxY = 1;
    y_ = randomInt(0, maxY);

    for (int i = 0; i < length_; ++i) {
        int color = 10;
        if (epilepsyMode_ && !randomColorPerSymbol_)
            color = lineColor_ = chooseColor();
        symbols_.emplace_back(static_cast<char>('0' + randomInt(0, 9)), color);
    }
}

int Line::chooseColor() const
{
    const int colors[] = { 9, 11, 12, 13, 14 };
    return colors[std::rand() % 5];
}

int Line::symbolY(int index, int headXPosition) const
{
    int height = ConsoleHelper::height();
    if (height < 3) return 0;

    int currentX = headXPosition + index;

    int totalPhase = (currentX + phase_) % 4;
    if (totalPhase < 0) totalPhase += 4; 

    int offset = 0;
    if (totalPhase == 0) offset = 0;
    else if (totalPhase == 1) offset = 1;
    else if (totalPhase == 2) offset = 2;
    else if (totalPhase == 3) offset = 1;

    int targetY = y_ + offset;
    if (targetY >= height) return height - 1;
    if (targetY < 0) return 0;

    return targetY;
}

bool Line::step(int screenWidth, int screenHeight)
{
    if (finished_) return false;

    lastHeadX_ = headX_;

    --headX_;

    if (headX_ < -length_) {
        finished_ = true;
        return false;
    }
    return true;
}

void Line::draw()
{
    for (int i = 0; i < length_; ++i) {
        int x = headX_ + i;
        if (x < 0 || x >= ConsoleHelper::width()) continue;

        int y = symbolY(i, headX_);
        if (y < 0 || y >= ConsoleHelper::height()) continue;

        int color = 10;
        if (epilepsyMode_) {
            color = randomColorPerSymbol_ ? chooseColor() : lineColor_;
        }
        else {
            color = 10;
        }

        ConsoleHelper::moveCursor(x, y);
        ConsoleHelper::setColor(color);
        std::cout << symbols_[i].value() << std::flush;
    }
    ConsoleHelper::resetColor();
}

void Line::eraseTail()
{
    int oldTailX = lastHeadX_ + (length_ - 1);

    if (oldTailX >= 0 && oldTailX < ConsoleHelper::width()) {
        int oldTailY = symbolY(length_ - 1, lastHeadX_);

        if (oldTailY >= 0 && oldTailY < ConsoleHelper::height()) {
            ConsoleHelper::moveCursor(oldTailX, oldTailY);
            std::cout << ' ' << std::flush;
        }
    }
}

bool Line::finished() const { return finished_; }
