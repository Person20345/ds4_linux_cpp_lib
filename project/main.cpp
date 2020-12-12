#include "include/headers.h"

// sudo apt install libpcre++-dev
// g++ main.cpp ds4.cpp -std=c++17 -lpcre -pthread -o test.out
// sudo ./test.out

void rgb(dsll::ds4 * d) {
    int sleep_time = 3;
    while(true) {
        for (int i = 0; i < 256; i++) {
            d->led.set_color(i, 0, 255-i);
            std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
        }
        for (int i = 0; i < 256; i++) {
            d->led.set_color(255-i, i, 0);
            std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
        }
        for (int i = 0; i < 256; i++) {
            d->led.set_color(0, 255-i, i);
            std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
        }
    }
}

int main() {
    
    dsll::ds4 D;
    std::thread th(rgb, &D);
    th.detach();

    for (int i = 0; i < 10; i++) {
        std::cout << i << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    return 0;
}