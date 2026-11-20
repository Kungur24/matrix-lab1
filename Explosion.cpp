#include "Explosion.h"
#include "ConsoleHelper.h"
#include <cstdlib>
#include <iostream>
#include <cmath>

namespace
{
    int randomInt(int minVal, int maxVal)
    {
        return minVal + std::rand() % (maxVal - minVal + 1);
    }

    char randomSymbol()
    {
        const char chars[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        return chars[std::rand() % (sizeof(chars) - 1)];
    }

    int randomColor()
    {
        // Яркие цвета консоли Windows (исключая черный)
        return randomInt(1, 15);
    }
}

Explosion::Explosion(int centerX, int centerY, int maxRadius)
    : centerX_(centerX),
    centerY_(centerY),
    currentRadius_(1),
    prevRadius_(0),
    maxRadius_(maxRadius),
    finished_(false),
    lastStepTime_(Clock::now()),
    stepInterval_(500) // 2 символа в секунду = 500 мс на шаг
{
}

std::vector<std::pair<int, int>> Explosion::getRingPoints(int radius) const
{
    std::vector<std::pair<int, int>> points;
    if (radius <= 0) return points;

    // Форма ромба (Рис. 1): |dx| + |dy| == radius
    for (int dy = -radius; dy <= radius; ++dy)
    {
        int dx = radius - std::abs(dy);
        if (dx == 0)
        {
            points.emplace_back(centerX_, centerY_ + dy);
        }
        else
        {
            points.emplace_back(centerX_ - dx, centerY_ + dy);
            points.emplace_back(centerX_ + dx, centerY_ + dy);
        }
    }
    return points;
}

bool Explosion::update(Clock::time_point now, int screenWidth, int screenHeight)
{
    if (finished_) return false;

    if (now - lastStepTime_ < stepInterval_)
    {
        return false;
    }

    lastStepTime_ = now;
    prevRadius_ = currentRadius_;
    ++currentRadius_;

    if (currentRadius_ > maxRadius_)
    {
        finished_ = true;
    }

    return true;
}

void Explosion::erase() const
{
    int radiusToErase = finished_ ? prevRadius_ : prevRadius_;
    if (radiusToErase <= 0) return;

    auto points = getRingPoints(radiusToErase);
    int width = ConsoleHelper::width();
    int height = ConsoleHelper::height();

    for (const auto& pt : points)
    {
        if (pt.first >= 0 && pt.first < width && pt.second >= 0 && pt.second < height)
        {
            ConsoleHelper::moveCursor(pt.first, pt.second);
            std::cout << ' ' << std::flush;
        }
    }
}

void Explosion::draw() const
{
    if (finished_) return;

    auto points = getRingPoints(currentRadius_);
    int width = ConsoleHelper::width();
    int height = ConsoleHelper::height();

    for (const auto& pt : points)
    {
        if (pt.first >= 0 && pt.first < width && pt.second >= 0 && pt.second < height)
        {
            ConsoleHelper::moveCursor(pt.first, pt.second);
            ConsoleHelper::setColor(randomColor());
            std::cout << randomSymbol() << std::flush;
        }
    }
    ConsoleHelper::resetColor();
}

bool Explosion::finished() const
{
    return finished_;
}
