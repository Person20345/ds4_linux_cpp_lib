#ifndef ds4_h
#define ds4_h
#include "headers.h"

/*
    ds4_linux_cpp_lib
    Writed by the_lll_end 08.12.2020

    namespace with class dualshock 4
*/

namespace dualshock4 {

    class ds4 {
    private:
        // /sys/class/leds/0000:0000:0000.0000:
        std::string led_path;
        // /sys/class/power_supply/sony_controller_battery_00:00:00:00:00:00/capacity
        std::string battery_status_path;
        static uint8_t ds4_counter;
        uint8_t ds4_index;
        bool is_connected;

        /*
            FUNCTIONS
        */

        // get path
        std::string get_path_to_file(std::string regular_expression, int size, std::string directory);
        // get leds path to change color
        int get_leds_paths();
        // get battery path to get status from it
        int get_battery_path();
        // change connection if we lose it
        int reconnect();
        // get status (0 - 100%) of battery
        uint8_t get_battery_status();
    public:
        // constructor
        ds4() {
            led_path = "/sys/class/leds/0000:0000:0000.0000:";
            reconnect();
            // start thread with events
        }

        // destructor
        void destruct();

        int led_set(uint8_t R, uint8_t G, uint8_t B);
    };
}
#endif 