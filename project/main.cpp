#include "headers.h"
#include "ds4.h"

//g++ main.cpp ds4.cpp -std=c++17 -lpcre -pthread

void rgb(dsll::ds4 * d) {
    int sleep_time = 1;
    int ret = 0;
    while(true) {
        for (int i = 0; i < 256; i++) {
            ret = d->set_color(i, 0, 255-i);
            if (ret < 0)
                break;
            std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
        }
        for (int i = 0; i < 256; i++) {
            ret = d->set_color(255-i, i, 0);
            if (ret < 0)
                break;
            std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
        }
        for (int i = 0; i < 256; i++) {
            ret = d->set_color(0, 255-i, i);
            if (ret < 0)
                break;
            std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
        }
        if (ret < 0)
                break;
    }
    std::cout << "ERROR" << std::endl;
}

int main() {
    
    dsll::ds4 D;
    D.connect();
    std::thread th(rgb, &D);
    th.detach();

    for (int i = 0; i < 10; i++) {
        std::cout << "\t\t\t" << i << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    return 0;
}