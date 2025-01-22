#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "gpioevent.h"

class EventPrinter : public GPIOPin::GPIOEventCallbackInterface {
	virtual void hasEvent(Event e) override {
		printf("%d\n",e);
	}
};

int main(int argc, char *argv[]) {
	fprintf(stderr,"Press any key to stop.\n");
	EventPrinter callbackInterface;
	GPIOPin gpiopin;
	gpiopin.registerCallback(&callbackInterface);
	gpiopin.start();
	getchar();
	gpiopin.stop();
	return 0;
}
