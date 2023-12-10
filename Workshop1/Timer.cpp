#include "Timer.h"

bool Timer::running_ = false;  // Define the static member variable

Timer::Timer() = default;

Timer::~Timer() {
    stopTimer();
}

// Correctly declare the copy constructor
Timer::Timer(const Timer&) {
    // Your implementation here, or delete it if you don't want to allow copying
}

// Correctly declare the copy assignment operator
Timer& Timer::operator=(const Timer&) {
    // Your implementation here, or delete it if you don't want to allow assignment
    return *this;
}

void Timer::startTimer(int intervalSeconds, std::function<void()> callback) {
    stopTimer();  // Stop any existing timer
    running_ = true;
    std::thread([this, intervalSeconds, callback]() {
        while (running_) {
            std::this_thread::sleep_for(std::chrono::seconds(intervalSeconds));
            if (running_) {
                callback();
            }
        }
        }).detach();
}

Timer& Timer::getInstance() {
    static Timer instance;
    return instance;
}

void Timer::stopTimer() {
    running_ = false;
}
