#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <pcre.h> // regular expressions
#include <unistd.h> // sleep()

//g++ main.cpp -std=c++17 -lpcre

class ds4 {
private:
    // /sys/class/leds/0000:0000:0000.0000:
    std::string led_path;
    // /sys/class/power_supply/sony_controller_battery_00:00:00:00:00:00/capacity
    std::string battery_status_path;
    static uint8_t ds4_counter;
    uint8_t ds4_index;
    bool is_connected;

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
    void destruct() {
        led_path.clear();
        battery_status_path.clear();
    }

    int led_set(uint8_t R, uint8_t G, uint8_t B);
};

uint8_t ds4::ds4_counter = 0;

std::string ds4::get_path_to_file(std::string pattern_, int size, std::string directory) {
    // set pattern
    char * pattern = new char[pattern_.length()];
    for (int i = 0; i < pattern_.length(); i++) {
        pattern[i] = pattern_[i];
    }

    // some regulars 
    pcre *regex;
    const char *error;
    int erroffset;
    regex = pcre_compile((char *) pattern, 0, &error, &erroffset, NULL);
    
    if (!regex) {
        return "";
    } else {
        std::filesystem::path path_;
        char * char_path;
        for (const auto & entry : std::filesystem::directory_iterator(directory)) {
            // set path
            path_ = entry.path();
            std::string str_path{path_.u8string()};
            char_path = new char [str_path.length()];
            for (int i = 0; i < str_path.length(); i++) {
                char_path[i] = str_path[i];
            }
            
            // find pattern in string
            int count = 0;
            int ovector[30];
            count = pcre_exec(regex, NULL, (char *) char_path, size, 0, 0, ovector, 30);
            if(count > 0) {
                return str_path;
            }
        }
    }
    return "";
}

int ds4::get_leds_paths() {
    std::string path = get_path_to_file("^.{16}\\w{4}:\\w{4}:\\w{4}\\.\\w{4}:red$", 39, "/sys/class/leds");
    if (path != "") {
        for (int i = 16; i < 35; i++) {
            led_path[i] = path[i];
        }
        std::cout << std::endl;
        return 1;
    } else {
        return -1;
    }
}

int ds4::get_battery_path() {
    std::string path = get_path_to_file("^(.{48})((([a-zA-Z0-9]){2}):){5}(([a-zA-Z0-9]){2})$", 65, "/sys/class/power_supply");
    if (path != "") {
        battery_status_path = path + "/capacity";
        return 1;
    } else {
        return -1;
    }
}

int ds4::reconnect() {
    if (is_connected == true) {
        ds4_counter--;
        is_connected = false;
    }
    if (get_leds_paths() > 0 && get_battery_path() > 0) {
        ds4_counter++;
        is_connected = true;
        return 1;
    } else {
        return -1;
    }
}

int ds4::led_set(uint8_t R, uint8_t G, uint8_t B) {
    if (is_connected == true) {
        std::ofstream led_r, led_g, led_b;
        led_r.open(led_path + "red/brightness");
        led_g.open(led_path + "green/brightness");
        led_b.open(led_path + "blue/brightness");
        if (!led_r || !led_g || !led_b) {
            return  -1;
        } else {
            led_r << std::to_string(R);
            led_g << std::to_string(G);
            led_b << std::to_string(B);   
        }
        led_r.close();
        led_b.close();
        led_g.close();
        return 1;   
    } else {
        return -1;
    }
}

void rgb(ds4 * d) {
    int sleep_time = 1000;
    int ret = 0;
    while(true) {
        for (int i = 0; i < 256; i++) {
            ret = d->led_set(i, 0, 255-i);
            if (ret < 0)
                break;
            usleep(sleep_time);
        }
        for (int i = 0; i < 256; i++) {
            d->led_set(255-i, i, 0);
            if (ret < 0)
                break;
            usleep(sleep_time);
        }
        for (int i = 0; i < 256; i++) {
            d->led_set(0, 255-i, i);
            if (ret < 0)
                break;
            usleep(sleep_time);
        }
        if (ret < 0)
                break;
    }
    std::cout << "ERROR" << std::endl;
}

int main() {
    ds4 D;
    rgb(&D);
    return 0;
}