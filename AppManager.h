#pragma once

class AppManager
{
public:
    int run(int argc, char* argv[]);

private:
    bool readInteractive(int& speed, int& length, bool& epilepsy);
    bool parseArguments(int argc, char* argv[], int& speed, int& length, bool& epilepsy);
    void showHelp() const;
    void animate(int speed, int length, bool epilepsy);
};
