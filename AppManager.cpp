#include "AppManager.h"
#include "ConsoleHelper.h"
#include "Line.h"

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>

namespace
{
    bool parseInteger(const char* text, int& value)
    {
        std::stringstream stream(text);
        stream >> value;
        return stream && stream.eof();
    }

    bool validYesNo(const std::string& value)
    {
        return value == "Y" || value == "y" ||
            value == "N" || value == "n";
    }
}

void AppManager::showHelp() const
{
    std::cout
        << "Matrix - Laboratory work 1, variant 13\n\n"
        << "Usage:\n"
        << "  matrix.exe\n"
        << "  matrix.exe SPEED LENGTH Y|N\n"
        << "  matrix.exe --help\n"
        << "  matrix.exe /?\n\n"
        << "SPEED: 1..30 characters per second\n"
        << "LENGTH: 1..30 characters\n"
        << "Y/N: epilepsy mode on/off\n"
        << "Movement: right to left\n"
        << "Shape: zig-zag\n";
}

bool AppManager::parseArguments(
    int argc, char* argv[],
    int& speed, int& length, bool& epilepsy)
{
    if (argc == 2 &&
        (std::string(argv[1]) == "--help" ||
            std::string(argv[1]) == "/?"))
    {
        showHelp();
        return false;
    }

    if (argc != 4)
    {
        std::cerr << "Expected SPEED LENGTH Y/N.\n";
        showHelp();
        return false;
    }

    if (!parseInteger(argv[1], speed) ||
        speed < 1 || speed > 30)
    {
        std::cerr << "Speed must be from 1 to 30.\n";
        return false;
    }

    if (!parseInteger(argv[2], length) ||
        length < 1 || length > 30)
    {
        std::cerr << "Length must be from 1 to 30.\n";
        return false;
    }

    std::string mode = argv[3];

    if (!validYesNo(mode))
    {
        std::cerr << "Epilepsy mode must be Y or N.\n";
        return false;
    }

    epilepsy = (mode == "Y" || mode == "y");
    return true;
}

bool AppManager::readInteractive(
    int& speed, int& length, bool& epilepsy)
{
    std::string input;

    while (true)
    {
        std::cout << "Line speed (1-30): ";
        std::getline(std::cin, input);

        std::stringstream stream(input);
        char extra;

        if ((stream >> speed) &&
            !(stream >> extra) &&
            speed >= 1 && speed <= 30)
        {
            break;
        }

        std::cout << "Enter an integer from 1 to 30.\n";
    }

    while (true)
    {
        std::cout << "Line length (1-30): ";
        std::getline(std::cin, input);

        std::stringstream stream(input);
        char extra;

        if ((stream >> length) &&
            !(stream >> extra) &&
            length >= 1 && length <= 30)
        {
            break;
        }

        std::cout << "Enter an integer from 1 to 30.\n";
    }

    while (true)
    {
        std::cout << "Epilepsy mode (Y/N): ";
        std::getline(std::cin, input);

        if (validYesNo(input))
        {
            epilepsy = (input == "Y" || input == "y");
            break;
        }

        std::cout << "Enter Y or N.\n";
    }

    return true;
}

void AppManager::animate(int speed, int length, bool epilepsy)
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    ConsoleHelper::clear();
    ConsoleHelper::hideCursor();

    const int delay = 1000 / speed;

    while (true)
    {
        Line line(length, speed, epilepsy, true);

        while (line.step(
            ConsoleHelper::width(),
            ConsoleHelper::height()))
        {
            line.draw();
            ConsoleHelper::sleepMilliseconds(delay);
            line.eraseTail();
        }

        ConsoleHelper::sleepMilliseconds(100);
    }
}

int AppManager::run(int argc, char* argv[])
{
    int speed = 10;
    int length = 10;
    bool epilepsy = false;

    if (argc == 1)
    {
        readInteractive(speed, length, epilepsy);
    }
    else
    {
        if (!parseArguments(argc, argv, speed, length, epilepsy))
            return 0;
    }

    animate(speed, length, epilepsy);
    return 0;
}
