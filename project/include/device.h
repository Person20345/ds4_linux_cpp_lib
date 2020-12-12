#ifndef device_h
#define device_h
#include "headers.h"

class dsll::device {
    protected:
        std::string path;
        bool is_connected;

        virtual int get_path() = 0;

        bool connection_status;
        std::thread connection_thread;
        static void connection_function(device * dualshock);

        int connect();
        
    public:
        device();
        ~device();
};

#endif 