#ifndef battery_h
#define battery_h
#include "headers.h"

class dsll::battery : public dsll::device {
    private:
        // path = "/sys/class/power_supply/sony_controller_battery_00:00:00:00:00:00/capacity"
        int get_path();
        
    public:
        battery();
        
        int status();
};

#endif 