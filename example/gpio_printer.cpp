#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "gpioevent.h"

class EventPrinter : public GPIOPin::GPIOEventCallbackInterface {
	virtual void hasEvent(const gpiod::edge_event& e) override {
	    switch (e.type()) {
			case gpiod::edge_event::event_type::RISING_EDGE:
				printf("Rising!\n");
				break;
			case gpiod::edge_event::event_type::FALLING_EDGE:
				printf("Falling\n");
				break;
		}
	}
};

int main(int argc, char *argv[]) {
	fprintf(stderr,"Press any key to stop.\n");
	EventPrinter callbackInterface;
	GPIOPin gpiopin;
	gpiopin.registerCallback(&callbackInterface);
	const int gpioPinNo = 27;
	gpiopin.start(gpioPinNo);
	getchar();
	gpiopin.stop();
	return 0;
}
