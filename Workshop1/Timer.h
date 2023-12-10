#include <functional>
#include <thread>
#include <chrono>

#ifndef Timer_H
#define Timer_H

class Timer
{
public:
    static Timer& getInstance();

    void startTimer(int intervalSeconds, std::function<void()> callback);

    void stopTimer();

private:
    Timer();
    ~Timer();

    Timer(const Timer&);
    Timer& operator=(const Timer&);

    static bool running_;

};
#endif
