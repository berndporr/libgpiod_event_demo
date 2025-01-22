#ifndef __GPIOEVENT_H
#define __GPIOEVENT_H

/*
 * Copyright (c) 2025  Bernd Porr <mail@berndporr.me.uk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 */
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <thread>
#include <gpiod.h>
#include <vector>

// enable debug messages and error messages to stderr
#ifndef NDEBUG
#define DEBUG
#endif

#define ISR_TIMEOUT 1000

// default GPIO chip
#define DEFAULT_CHIP 0

// default GPIO pin for the ALRT/DRY signal
#define DEFAULT_GPIO 17

class GPIOPin {

public:
    /**
     * Destructor which makes sure the data acquisition
     * stops on exit.
     **/
    ~GPIOPin() {
	stop();
    }

    enum Event { none, rising, falling };

    struct GPIOEventCallbackInterface {
	    /**
	     * Called when a new sample is available.
	     * This needs to be implemented in a derived
	     * class by the client. Defined as abstract.
	     * \param e If falling or rising.
	     **/
	virtual void hasEvent(Event e) = 0;
    };

    void registerCallback(GPIOEventCallbackInterface* ci) {
	adsCallbackInterfaces.push_back(ci);
    }

    /**
     * Starts listening on the GPIO pin.
     * \param settings A struct with the settings.
     **/
    void start(int drdy_chip = DEFAULT_CHIP,
	       int drdy_gpio = DEFAULT_GPIO);

    /**
     * Stops listening to the pin.
     **/
    void stop();

private:
    void gpioEvent(struct gpiod_line_event event);

    void worker();

    // gpiod stuff
    struct gpiod_chip *chipDRDY = nullptr;
    struct gpiod_line *pinDRDY = nullptr;

    // thread
    std::thread thr;

    // flag that it's running
    bool running = false;

    std::vector<GPIOEventCallbackInterface*> adsCallbackInterfaces;
};


#endif
