#include "include/headers.h"

void dsll::rgb(dsll::ds4 * d) {
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

void dsll::rgb_thread(std::thread * th, dsll::ds4 * dualshock) {
    std::thread loop(rgb, dualshock);
    loop.detach();
    th = &loop;
}
