
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
    std::cout << "ERROR: " << ret << std::endl;
}
