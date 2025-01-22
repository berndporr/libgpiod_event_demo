#include "gpioevent.h"

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>


void GPIOPin::start(int drdy_chip,
		    int drdy_gpio) {
	
#ifdef DEBUG
    fprintf(stderr,"Init.\n");
#endif

    chipDRDY = gpiod_chip_open_by_number(drdy_chip);
    pinDRDY = gpiod_chip_get_line(chipDRDY,drdy_gpio);

    int ret = gpiod_line_request_rising_edge_events(pinDRDY, "Consumer");
    if (ret < 0) {
#ifdef DEBUG
	fprintf(stderr,"Request event notification failed on pin %d and chip %d.\n",
		settings.drdy_chip,settings.drdy_gpio);
#endif
	throw "Could not request event for IRQ.";
    }

    running = true;

    thr = std::thread(&GPIOPin::worker,this);
}

void GPIOPin::gpioEvent(struct gpiod_line_event event) {
	Event e;
	switch (gpiod_edge_event_get_event_type(event)) {
	case GPIOD_EDGE_EVENT_RISING_EDGE:
	    e = rising;
	    break;
	case GPIOD_EDGE_EVENT_FALLING_EDGE:
	    e = falling;
	    break;
	default:
	    e = none;
	}
	for(auto &cb: adsCallbackInterfaces) {
	    cb->hasADS1115Sample(v);
	}
}


void GPIOPin::worker() {
    while (running) {
	const struct timespec ts = { 1, 0 };
	gpiod_line_event_wait(pinDRDY, &ts);
	struct gpiod_line_event event;
	gpiod_line_event_read(pinDRDY, &event);
	gpioEvent(e);
    }
}


void GPIOPin::stop() {
    if (!running) return;
    running = false;
    thr.join();
    gpiod_line_release(pinDRDY);
    gpiod_chip_close(chipDRDY);
}
