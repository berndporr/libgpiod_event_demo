#ifndef __GPIOEVENT_H
#define __GPIOEVENT_H

/*
 * Copyright (c) 2025  Bernd Porr <mail@berndporr.me.uk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation. See the file LICENSE.
 */
#include <gpiod.hpp>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <thread>
#include <iostream>
#include <chrono>
#include <functional>
#include <vector>

// enable debug messages and error messages to stderr
#ifndef NDEBUG
#define DEBUG
#endif

#define ISR_TIMEOUT_MS 1000 // milisec

class GPIOPin {

public:
    /**
     * Destructor which makes sure the data acquisition
     * stops on exit.
     **/
    ~GPIOPin() {
	stop();
    }

    /**
     * Called after an GPIO event.
     * The parameter has a reference to the edge event class.
     **/
    using EventCallback = std::function<void(const gpiod::edge_event&)>;

    void registerCallback(EventCallback ec) {
	eventCallback = ec;
    }

    /**
     * Starts listening on the GPIO pin.
     * \param chipNo GPIO Chip number. It's usually 0.
     * \param pinNo GPIO Pin number.
     **/
    void start(int pinNo,
	       int ChipNo = 0);

    /**
     * Stops listening to the pin.
     **/
    void stop();

private:
    void gpioEvent(const gpiod::edge_event& event);

    void worker();

    // thread
    std::thread thr;

    // flag that it's running
    bool running = false;

    int _pinNo = 0;
    int _chipNo = 0;

    EventCallback eventCallback;

    const std::string consumername = "gpioeventconsumer";
};


#endif
