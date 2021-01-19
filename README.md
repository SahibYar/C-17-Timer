# C-17-Timer
JavaScript Like thread-safe Header only C++17 **Timer** Library using `std::thread` and `std::chrono` without polling.

## Features
1. It takes `const std::chrono::duration<Rep, Period> &duration` as a duration so by default supports all the [std::chrono::durations](https://en.cppreference.com/w/cpp/chrono/duration)
2. It takes `Function &&f, Args &&... args` which is the same as [std::thread](https://en.cppreference.com/w/cpp/thread/thread) constructor
3. It can notify the caller via `std::condition_variable`, so you do not need to do polling.


### Example 1: 
It can take any duration, with `repeat` and `once`, for stopping repeating
```
Timer t = Timer();
t.once(std::chrono::seconds (5), []() { std::cout << "Will be printed once after 5 seconds" << std::endl;});
t.once(std::chrono::milliseconds(1500), []() { std::cout << "Will be printed once after 1500 milliseconds" << std::endl;});

t.repeat(std::chrono::milliseconds(1500), []() { std::cout << "Will be printed repeatedly after 1500 milliseconds" << std::endl;});

// for stop repeating
t.stop();
```


### Example 2: 
It can take any number of parameters
```
void func1(const std::string_view str, const std::string_view str2) {
    std::cout << str << str2 << std::endl;
}

Timer t = Timer();
t.repeat(std::chrono::microseconds (1100), func2, "This is the value", "this is my 2nd parameter");
```

### Example 3:
```
#include <condition_variable>
#include <mutex>

void cv_func(bool& processed, std::condition_variable& cv)
{
    //do some processing here.
    processed = true;
    cv.notify_all();
}

int main()
{
    bool processed = false;
    std::condition_variable cv;
    std::mutex m;

    t.once(std::chrono::microseconds (1000), cv_func, std::ref(processed), std::ref(cv));
    std::unique_lock<std::mutex> lk(m);
    
    // no need to do polling / busy waiting.
    cv.wait(lk, [&] { return processed; });
   
    std::cout << "This line will be executed only when condition variable is set to true in a thread." << std::endl;
}
```
