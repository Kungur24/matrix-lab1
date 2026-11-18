#include "AppManager.h"
#include "ConsoleHelper.h"
#include "Line.h"

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <memory>
#include <algorithm>
#include <chrono>

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

    int readValidatedInt(const std::string& prompt, int minVal, int maxVal)
    {
        std::string input;
        int value = 0;

        while (true)
        {
            std::cout << prompt;
            std::getline(std::cin, input);

            std::stringstream stream(input);
            char extra;

            if ((stream >> value) &&
                !(stream >> extra) &&
                value >= minVal && value <= maxVal)
            {
                return value;
            }

            std::cout << "Enter an integer from " << minVal << " to " << maxVal << ".\n";
        }
    }
}

void AppManager::showHelp() const
{
    std::cout
        << "Matrix - Laboratory work 2, variant 13\n\n"
        << "Usage:\n"
        << "  matrix.exe\n"
        << "  matrix.exe FREQUENCY SPEED LENGTH Y|N\n"
        << "  matrix.exe --help\n"
        << "  matrix.exe /?\n\n"
        << "FREQUENCY: 1..30 lines per second\n"
        << "SPEED: 1..30 characters per second\n"
        << "LENGTH: 1..30 characters\n"
        << "Y/N: epilepsy mode on/off\n"
        << "Movement: right to left\n"
        << "Shape: zig-zag\n";
}

bool AppManager::parseArguments(
    int argc, char* argv[],
    int& frequency, int& speed, int& length, bool& epilepsy)
{
    if (argc == 2 &&
        (std::string(argv[1]) == "--help" ||
            std::string(argv[1]) == "/?"))
    {
        showHelp();
        return false;
    }

    if (argc != 5)
    {
        std::cerr << "Expected FREQUENCY SPEED LENGTH Y/N.\n";
        showHelp();
        return false;
    }

    if (!parseInteger(argv[1], frequency) ||
        frequency < 1 || frequency > 30)
    {
        std::cerr << "Frequency must be from 1 to 30.\n";
        return false;
    }

    if (!parseInteger(argv[2], speed) ||
        speed < 1 || speed > 30)
    {
        std::cerr << "Speed must be from 1 to 30.\n";
        return false;
    }

    if (!parseInteger(argv[3], length) ||
        length < 1 || length > 30)
    {
        std::cerr << "Length must be from 1 to 30.\n";
        return false;
    }

    std::string mode = argv[4];

    if (!validYesNo(mode))
    {
        std::cerr << "Epilepsy mode must be Y or N.\n";
        return false;
    }

    epilepsy = (mode == "Y" || mode == "y");
    return true;
}

bool AppManager::readInteractive(
    int& frequency, int& speed, int& length, bool& epilepsy)
{
    frequency = readValidatedInt("Line frequency (1-30): ", 1, 30);
    speed = readValidatedInt("Line speed (1-30): ", 1, 30);
    length = readValidatedInt("Line length (1-30): ", 1, 30);

    std::string input;
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

std::vector<int> AppManager::generateSpawnDelays(int frequency) const
{
    std::vector<int> delays;
    delays.reserve(frequency);

    for (int i = 0; i < frequency; ++i)
    {
        delays.push_back(std::rand() % 1001);
    }

    std::sort(delays.begin(), delays.end());
    return delays;
}

void AppManager::animate(int frequency, int speed, int length, bool epilepsy)
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    ConsoleHelper::clear();
    ConsoleHelper::hideCursor();

    using Clock = std::chrono::steady_clock;

    std::vector<std::unique_ptr<Line>> lines;
    auto secondStart = Clock::now();
    std::vector<int> spawnDelays = generateSpawnDelays(frequency);
    size_t nextSpawnIndex = 0;

    while (true)
    {
        auto now = Clock::now();
        auto elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(now - secondStart).count();

        if (elapsedMs >= 1000)
        {
            while (nextSpawnIndex < spawnDelays.size())
            {
                lines.push_back(std::make_unique<Line>(length, speed, epilepsy, true));
                ++nextSpawnIndex;
            }

            secondStart = now;
            spawnDelays = generateSpawnDelays(frequency);
            nextSpawnIndex = 0;
            elapsedMs = 0;
        }

        while (nextSpawnIndex < spawnDelays.size() && elapsedMs >= spawnDelays[nextSpawnIndex])
        {
            lines.push_back(std::make_unique<Line>(length, speed, epilepsy, true));
            ++nextSpawnIndex;
        }

        int screenW = ConsoleHelper::width();
        int screenH = ConsoleHelper::height();
        bool anyMoved = false;

        for (auto& line : lines)
        {
            if (line->update(now, screenW, screenH))
            {
                line->eraseTail();
                anyMoved = true;
            }
        }

        if (anyMoved)
        {
            for (const auto& line : lines)
            {
                line->draw();
            }
        }

        lines.erase(
            std::remove_if(lines.begin(), lines.end(),
                [](const std::unique_ptr<Line>& l) { return l->finished(); }),
            lines.end());

        ConsoleHelper::sleepMilliseconds(5);
    }
}

int AppManager::run(int argc, char* argv[])
{
    int frequency = 5;
    int speed = 10;
    int length = 10;
    bool epilepsy = false;

    if (argc == 1)
    {
        readInteractive(frequency, speed, length, epilepsy);
    }
    else
    {
        if (!parseArguments(argc, argv, frequency, speed, length, epilepsy))
            return 0;
    }

    animate(frequency, speed, length, epilepsy);
    return 0;
}
