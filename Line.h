#pragma once
#include <vector>
#include "Figure.h"
#include "Symbol.h"

class Line : public Figure
{
public:
    Line(int length, int speed, bool epilepsyMode, bool randomColorPerSymbol);

    bool update(Clock::time_point now, int screenWidth, int screenHeight) override;
    void draw() const override;
    void erase() const override;
    bool finished() const override;

    // Методы для механики взрыва
    int headX() const;
    int headY() const;
    bool isHeadOnScreen() const;
    void shrinkByOne();

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
