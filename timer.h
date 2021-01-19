/*
 * sahibyaar@hotmail.com
 * Date 18-Jan-2021
 * Inspired by:
 * https://www.fluentcpp.com/2018/12/28/timer-cpp/
 * */

#ifndef CMS_TIMER
#define CMS_TIMER

#include <thread>
#include <chrono>
#include <atomic>
#include <condition_variable>

class Timer {
    std::mutex m;
    std::atomic_bool clear = false;

public:
    template<class Function, class... Args, class Rep, class Period>
    [[maybe_unused]] void once(const std::chrono::duration<Rep, Period> &sleep_duration, Function &&f, Args &&... args);

    template<class Function, class... Args, class Rep, class Period>
    [[maybe_unused]] void repeat(const std::chrono::duration<Rep, Period> &sleep_duration, Function &&f, Args &&... args);

    [[maybe_unused]] void stop();
};

template<class Function, class... Args, class Rep, class Period>
void Timer::once(const std::chrono::duration<Rep, Period> &sleep_duration, Function &&f, Args &&... args) {
    this->clear = false;
    std::thread t([&, f = std::forward<Function>(f), args = std::make_tuple(std::forward<Args>(args)...)]() {
        if (this->clear) return;
        std::this_thread::sleep_until(std::chrono::steady_clock::now() + sleep_duration);
        if (this->clear) return;
        std::apply(f, args);
    });
    t.detach();
}

template<class Function, class... Args, class Rep, class Period>
void Timer::repeat(const std::chrono::duration<Rep, Period> &sleep_duration, Function &&f, Args &&... args) {
    this->clear = false;
    std::thread t([&, f = std::forward<Function>(f), args = std::make_tuple(std::forward<Args>(args)...)]() {
        while (true) {
            if (this->clear) return;
            std::this_thread::sleep_until(std::chrono::steady_clock::now() + sleep_duration);
            if (this->clear) return;
            std::apply(f, args);
        }
    });
    t.detach();
}

void Timer::stop() {
    this->clear = true;
}
#endif