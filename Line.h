#pragma once
#include <vector>
#include <chrono>
#include "Symbol.h"

class Line
{
public:
    using Clock = std::chrono::steady_clock;

    Line(int length, int speed, bool epilepsyMode, bool randomColorPerSymbol);

    bool update(Clock::time_point now, int screenWidth, int screenHeight);
    void draw() const;
    void eraseTail() const;
    bool finished() const;

private:
    int length_;
    int speed_;
    int headX_;
    int lastHeadX_;
    int y_;
    int phase_;
    bool epilepsyMode_;
    bool randomColorPerSymbol_;
    bool finished_;
    int lineColor_;
    std::vector<Symbol> symbols_;

    Clock::time_point lastMoveTime_;
    std::chrono::milliseconds stepInterval_;

    int chooseColor() const;
    int symbolY(int index, int headXPosition) const;
};
