#ifndef leds_h
#define leds_h
#include "headers.h"

class dsll::leds : public dsll::device {
    private:
        dsll::color main_color;

        // path = "/sys/class/leds/0000:0000:0000.0000:"
        int get_path();

    public:
        leds();

        int set_color(uint8_t R, uint8_t G, uint8_t B);
        int set_color(dsll::color color_);

        dsll::color get_color();
};

#endif 