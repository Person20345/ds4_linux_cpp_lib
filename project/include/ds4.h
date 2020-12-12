#ifndef ds4_h
#define ds4_h
#include "headers.h"

class dsll::ds4 : public dsll::device {
private:
    int index;

    // path = "/dev/input/js0"
    int get_path();

public:
    dsll::leds led;
    dsll::battery batt;

    ds4();
};

#endif 