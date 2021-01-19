#include <iostream>
#include <string>

#include "timer.h"
using namespace std;

void func1() {
    cout << "Hello from func1" << endl;
}

void func2(const std::string_view str, const std::string_view str2) {

    cout << str << str2 <<endl;
}

#include <condition_variable>
#include <mutex>

void cv_func(bool& processed, std::condition_variable& cv)
{
    processed = true;
    cv.notify_all();
}

int main() {

    Timer t = Timer();

    t.repeat(std::chrono::milliseconds(1000), func2, "This is the value", "this is my 2nd parameter");
    t.once(std::chrono::seconds (1000), []() { std::cout << "This is my timer" << std::endl;});

    {
        bool processed = false;
        std::condition_variable cv;
        std::mutex m;

        t.once(std::chrono::microseconds (1000), cv_func, std::ref(processed), std::ref(cv));
        std::unique_lock<std::mutex> lk(m);
        cv.wait(lk, [&] { return processed; });
    }
    std::cout << "MAIN MAIN MAIN" << std::endl;

   std::this_thread::sleep_for(std::chrono::milliseconds (5000));
    t.stop();
}
