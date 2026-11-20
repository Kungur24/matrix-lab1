#pragma once
#include <vector>

class AppManager
{
public:
    int run(int argc, char* argv[]);

private:
    bool readInteractive(int& frequency, int& speed, int& length, bool& epilepsy,
        int& explosionProb, int& minRadius, int& maxRadius);
    bool parseArguments(int argc, char* argv[], int& frequency, int& speed, int& length, bool& epilepsy,
        int& explosionProb, int& minRadius, int& maxRadius);
    void showHelp() const;
    void animate(int frequency, int speed, int length, bool epilepsy,
        int explosionProb, int minRadius, int maxRadius);
    std::vector<int> generateSpawnDelays(int frequency) const;
};
