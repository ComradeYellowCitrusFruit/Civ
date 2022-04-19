#ifndef CIVILIZATION_CUSTOM_TIME_FUNCTIONS_HPP
#define CIVILIZATION_CUSTOM_TIME_FUNCTIONS_HPP

class timer
{
    public:
        int getTime();
        void startTimer();
        void endTimer();
    private:
        int startTime;
        int endTime;
        int duration;
        bool running;
};

double sinTime();
double cosTime();

#endif