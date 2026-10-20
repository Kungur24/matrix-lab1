#pragma once

class Symbol
{
public:
    Symbol(char value = '0', int color = 10);

    char value() const;
    int color() const;

private:
    char value_;
    int color_; 
};
