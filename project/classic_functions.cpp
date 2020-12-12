#include "include/headers.h"

/*
 *  ds4_linux_cpp_lib
 *  Writed by the_lll_end 08.12.2020
 *
 *  All classic methods realisation
*/

// FUNCTIONS

std::string dsll::get_path_to_file(std::string regular_expression, 
                                   int size, 
                                   std::string directory) {
    char * pattern = new char[regular_expression.length()];
    for (int i = 0; i < regular_expression.length(); i++) {
        pattern[i] = regular_expression[i];
    }

    pcre *regex;
    const char *error;
    int erroffset;
    regex = pcre_compile((char *) pattern, 0, &error, &erroffset, NULL);
    
    if (regex) {
        std::filesystem::path fs_path;
        char * char_path;
        for (const auto & entry : std::filesystem::directory_iterator(directory)) {
            fs_path = entry.path();
            std::string str_path{fs_path.u8string()};
            char_path = new char [str_path.length()];
            for (int i = 0; i < str_path.length(); i++) {
                char_path[i] = str_path[i];
            }
            
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

// CLASS DEVICE

void dsll::device::connection_function(dsll::device * dev) {
    dev->connection_status = true;
    dev->is_connected = false;
    while (dev->connection_status == true) {
        if (dev->is_connected == false) {
            dev->connect();
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

dsll::device::device() : connection_thread{connection_function, this} {}

dsll::device::~device() {
    path.clear();
    connection_status = false; // to stop connection_thread
    is_connected = false;
}

int dsll::device::connect() {
    int ret = get_path();
    if (ret == 1) {
        is_connected = true;
    } else {
        is_connected = false;
    }
    return ret;
}

// CLASS LED

int dsll::leds::get_path() {
    this->path = "/sys/class/leds/0000:0000:0000.0000:";
    std::string path = get_path_to_file("^.{16}\\w{4}:\\w{4}:\\w{4}\\.\\w{4}:red$", 
                                        39, "/sys/class/leds");
    if (path != "") {
        for (int i = 16; i < 35; i++) {
            (this->path)[i] = path[i];
        }
        return 1;
    } else {
        return -1;
    }
}

dsll::leds::leds() : device() {}

int dsll::leds::set_color(uint8_t R, uint8_t G, uint8_t B) {
    if (R > 255) { R = 255; }
    if (G > 255) { G = 255; }
    if (B > 255) { B = 255; }

    if (is_connected == true) {
        std::ofstream led_r, led_g, led_b;
        led_r.open(path + "red/brightness");
        led_g.open(path + "green/brightness");
        led_b.open(path + "blue/brightness");
        if (!led_r || !led_g || !led_b) {
            is_connected = false;
            return  -2;
        } else {
            main_color.red = R;
            main_color.green = G;
            main_color.blue = B;
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

int dsll::leds::set_color(dsll::color color_) {
    return set_color(color_.red, color_.green, color_.blue);
}

dsll::color dsll::leds::get_color() { return main_color; }

// CLASS BATTERY

dsll::battery::battery() : device() {}

int dsll::battery::get_path() {
    this->path = "/sys/class/power_supply/sony_controller_battery_00:00:00:00:00:00/capacity";
    std::string path = get_path_to_file("^(.{48})((([a-zA-Z0-9]){2}):){5}(([a-zA-Z0-9]){2})$", 
                                        65, "/sys/class/power_supply");
    if (path != "") {
        this->path = path + "/capacity";
        return 1;
    } else {
        return -1;
    }
}

int dsll::battery::status() {
    std::ifstream file;
    file.open(path);
    if (file) {
        std::string data;
        uint8_t int_data;

        file >> data;
        int_data = std::stoi(data);
        if (int_data <= 100) {
            return int_data;
        } else {
            return -1;
        }
    } else {
        is_connected = false;
        return -1;
    }
}

// CLASS DS4

int dsll::ds4::get_path() {
    std::string path = get_path_to_file("^js\\d$", 65, "/dev/input");
    if (path != "") {
        std::string str_index;
        str_index = path[2];
        this->path = path;
        index = stoi(str_index);
        return 1;
    } else {
        return -1;
    }
}

dsll::ds4::ds4() : device() {
    is_connected = false;
}