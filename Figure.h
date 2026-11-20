#pragma once
#include <chrono>

class Figure
{
public:
    using Clock = std::chrono::steady_clock;

    virtual ~Figure() = default;

    virtual bool update(Clock::time_point now, int screenWidth, int screenHeight) = 0;
    virtual void draw() const = 0;
    virtual void erase() const = 0;
    virtual bool finished() const = 0;
};
