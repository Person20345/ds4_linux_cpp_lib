#include "include/headers.h"

// sudo apt install libpcre++-dev
// g++ main.cpp classic_functions.cpp led_functions.cpp -std=c++17 -lpcre -pthread -o ./test.out
// sudo ./test.out

int main() {
    dsll::ds4 d;
    std::thread * th;
    dsll::rgb_thread(th, &d);

    for (int i = 0; i < 10; i++) {
        std::cout << i << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    return 0;
}