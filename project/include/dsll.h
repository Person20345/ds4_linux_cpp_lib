#ifndef dsll_h
#define dsll_h
#include "headers.h"

namespace dsll {
    std::string get_path_to_file(std::string regular_expression, 
                                 int size, 
                                 std::string directory);

    class color;

    class device;

    class leds;

    class battery;

    class ds4;
}
#endif 