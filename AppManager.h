#pragma once
#include <vector>

class AppManager
{
public:
    int run(int argc, char* argv[]);

private:
    bool readInteractive(int& frequency, int& speed, int& length, bool& epilepsy);
    bool parseArguments(int argc, char* argv[], int& frequency, int& speed, int& length, bool& epilepsy);
    void showHelp() const;
    void animate(int frequency, int speed, int length, bool epilepsy);
    std::vector<int> generateSpawnDelays(int frequency) const;
};

