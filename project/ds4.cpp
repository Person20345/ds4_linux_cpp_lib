#include "headers.h"
#include "ds4.h"

/*
    ds4_linux_cpp_lib
    Writed by the_lll_end 08.12.2020

    All methods realisation
*/

//g++ main.cpp -std=c++17 -lpcre    
uint8_t dualshock4::ds4::ds4_counter = 0;

void dualshock4::ds4::destruct() {
    led_path.clear();
    battery_status_path.clear();
}

std::string dualshock4::ds4::get_path_to_file(std::string pattern_, int size, std::string directory) {
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

int dualshock4::ds4::get_leds_paths() {
    std::string path = get_path_to_file("^.{16}\\w{4}:\\w{4}:\\w{4}\\.\\w{4}:red$", 39, "/sys/class/leds");
    if (path != "") {
        for (int i = 16; i < 35; i++) {
            led_path[i] = path[i];
        }
        return 1;
    } else {
        return -1;
    }
}

int dualshock4::ds4::get_battery_path() {
    std::string path = get_path_to_file("^(.{48})((([a-zA-Z0-9]){2}):){5}(([a-zA-Z0-9]){2})$", 65, "/sys/class/power_supply");
    if (path != "") {
        battery_status_path = path + "/capacity";
        return 1;
    } else {
        return -1;
    }
}

int dualshock4::ds4::reconnect() {
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

int dualshock4::ds4::led_set(uint8_t R, uint8_t G, uint8_t B) {
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