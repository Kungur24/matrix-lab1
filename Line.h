#pragma once
#include <vector>
#include "Symbol.h"

class Line
{
public:
    Line(int length, int speed, bool epilepsyMode, bool randomColorPerSymbol);

    bool step(int screenWidth, int screenHeight);
    void draw();
    void eraseTail();
    bool finished() const;

private:
    int length_;
    int speed_;
    int headX_;
    int y_;
    int phase_;
    bool epilepsyMode_;
    bool randomColorPerSymbol_;
    bool finished_;
    int lineColor_;
    std::vector<Symbol> symbols_;

    int lastHeadX_;

    int chooseColor() const;
    int symbolY(int index, int headXPosition) const;
};
