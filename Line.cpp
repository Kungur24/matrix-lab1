#include "Line.h"
#include "ConsoleHelper.h"
#include <cstdlib>
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
    lineColor_(10),
    lastMoveTime_(Clock::now()),
    stepInterval_(1000 / speed)
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

bool Line::update(Clock::time_point now, int screenWidth, int screenHeight)
{
    if (finished_ || length_ <= 0)
    {
        finished_ = true;
        return false;
    }

    if (now - lastMoveTime_ < stepInterval_) {
        return false;
    }

    lastMoveTime_ = now;
    lastHeadX_ = headX_;
    --headX_;

    if (headX_ < -length_) {
        finished_ = true;
    }

    return true;
}

void Line::draw() const
{
    if (finished_ || length_ <= 0) return;

    for (int i = 0; i < length_; ++i) {
        int x = headX_ + i;
        if (x < 0 || x >= ConsoleHelper::width()) continue;

        int y = symbolY(i, headX_);
        if (y < 0 || y >= ConsoleHelper::height()) continue;

        int color = 10;
        if (epilepsyMode_) {
            color = randomColorPerSymbol_ ? chooseColor() : lineColor_;
        }

        ConsoleHelper::moveCursor(x, y);
        ConsoleHelper::setColor(color);
        std::cout << symbols_[i].value() << std::flush;
    }
    ConsoleHelper::resetColor();
}

void Line::erase() const
{
    if (length_ <= 0) return;

    int oldTailX = lastHeadX_ + (length_ - 1);

    if (oldTailX >= 0 && oldTailX < ConsoleHelper::width()) {
        int oldTailY = symbolY(length_ - 1, lastHeadX_);

        if (oldTailY >= 0 && oldTailY < ConsoleHelper::height()) {
            ConsoleHelper::moveCursor(oldTailX, oldTailY);
            std::cout << ' ' << std::flush;
        }
    }
}

bool Line::finished() const { return finished_ || length_ <= 0; }

int Line::headX() const { return headX_; }

int Line::headY() const { return symbolY(0, headX_); }

bool Line::isHeadOnScreen() const
{
    return !finished_ && length_ > 0 && headX_ >= 0 && headX_ < ConsoleHelper::width();
}

void Line::shrinkByOne()
{
    if (length_ <= 0 || symbols_.empty()) return;

    // Стираем текущий первый символ (центр взрыва)
    int hx = headX();
    int hy = headY();
    if (hx >= 0 && hx < ConsoleHelper::width() && hy >= 0 && hy < ConsoleHelper::height())
    {
        ConsoleHelper::moveCursor(hx, hy);
        std::cout << ' ' << std::flush;
    }

    // Удаляем первый символ и уменьшаем длину на 1
    symbols_.erase(symbols_.begin());
    --length_;
    ++headX_; // Следующий символ становится новой головой на своей прежней X-координате
    lastHeadX_ = headX_;

    if (length_ <= 0)
    {
        finished_ = true;
    }
}
