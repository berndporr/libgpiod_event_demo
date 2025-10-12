#include "gpioevent.h"

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>


void GPIOPin::start(int pinNo,
		    int chipNo) {
	
#ifdef DEBUG
    fprintf(stderr,"Init.\n");
#endif

    chip = gpiod::chip("gpiochip0");

    line_cfg.add_line_settings(
			       pinNo,
			       gpiod::line_settings()
			       .set_direction(gpiod::line::direction::INPUT)
			       .set_edge_detection(gpiod::line::edge::FALLING)
			       );

    req_cfg.set_consumer("gpiodemo");

    auto builder = chip.prepare_request();
    builder.set_request_config(req_cfg);
    builder.set_line_config(line_cfg);
    request = builder.do_request();
    
    running = true;

    thr = std::thread(&GPIOPin::worker,this);
}

void GPIOPin::gpioEvent(const gpiod::edge_event& event) {
	for(auto &cb: adsCallbackInterfaces) {
	    cb->hasEvent(event);
	}
}


void GPIOPin::worker() {
    while (running) {
	bool r = request.wait_edge_events(std::chrono::milliseconds(ISR_TIMEOUT_MS));
	if (r) {
	    // callback
	    gpiod::edge_event_buffer buffer;
	    request.read_edge_events(buffer,1);
	    gpioEvent(buffer.get_event(0));
	} else {
#ifdef DEBUG
	    fprintf(stderr,"Timeout!\n");
#endif
	}
    }
    request.release();
}


void GPIOPin::stop() {
    if (!running) return;
    running = false;
    thr.join();
}
