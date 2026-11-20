#pragma once
#include "Figure.h"
#include <vector>
#include <utility>

class Explosion : public Figure
{
public:
    Explosion(int centerX, int centerY, int maxRadius);

    bool update(Clock::time_point now, int screenWidth, int screenHeight) override;
    void draw() const override;
    void erase() const override;
    bool finished() const override;

private:
    int centerX_;
    int centerY_;
    int currentRadius_;
    int prevRadius_;
    int maxRadius_;
    bool finished_;

    Clock::time_point lastStepTime_;
    std::chrono::milliseconds stepInterval_;

    std::vector<std::pair<int, int>> getRingPoints(int radius) const;
};
