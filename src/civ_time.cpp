#include <ctime>
#include <cmath>
#include "civ_time.hpp"

class timer
{
    public:
        int getTime()
        {
            if(running)
            {
                return startTime - (int)time(NULL);
            }
            else
            {
                return duration;
            }
        }
        void startTimer()
        {
            running = true;
            startTime = (int)time(NULL);
        }
        void endTimer()
        {
            running = false;
            endTime = (int)time(NULL);
            duration = startTime - endTime;
        }
    private:
        int startTime;
        int endTime;
        int duration;
        bool running;
};

double sinTime()
{
    return sin((double)time(NULL));
}
double cosTime()
{
    return ((double)time(NULL));
}