#ifndef ds4_h
#define ds4_h
#include "headers.h"

/*
 *  ds4_linux_cpp_lib
 *  Writed by the_lll_end 08.12.2020
 *
 *  namespace with class dualshock 4
*/

namespace dsll {
    std::string get_path_to_file(std::string regular_expression, 
                                 int size, 
                                 std::string directory);

    class color {
    public:
        uint8_t red;
        uint8_t green;
        uint8_t blue;
    };

    class device {
    protected:
        std::string path;
        std::thread main_loop;
        bool is_connected;
        int sleep_time;
        int event;

        virtual int get_path() = 0;
        // this must be "virtual int events_function() = 0;"
        int events_function();

        static int main_loop_function(device * device_);
    public:
        device(int sleep_time);
        ~device();

        int connect();
    };

    class led : public device {
    private:
        // path = "/sys/class/leds/0000:0000:0000.0000:"
        
        color main_color;

        int get_path();
        void events_function();
    public:
        led();
        int set_color(uint8_t R, uint8_t G, uint8_t B);
        int set_color(color color_);

        void set_main_color(uint8_t R, uint8_t G, uint8_t B);
        void set_main_color(color main_color);

        color get_main_color();
    };

    class battery : public device {
    private:
        // path = "/sys/class/power_supply/sony_controller_battery_00:00:00:00:00:00/capacity"

        int get_path();
        void events_function();
    public:
        battery();
        uint8_t get_battery_status();
    };

    class ds4 : public device {
    private:
        led leds;
        battery batt;
        int index;

        int get_path();
    public:
        ds4();
        int set_color(uint8_t R, uint8_t G, uint8_t B);
        int set_color(color color_);
    };
}
#endif 