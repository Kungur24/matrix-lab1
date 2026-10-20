
#include "Symbol.h"

Symbol::Symbol(char value, int color)
    : value_(value), color_(color)
{
}

char Symbol::value() const
{
    return value_;
}

int Symbol::color() const
{
    return color_;
}