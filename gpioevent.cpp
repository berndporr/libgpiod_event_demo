#include "gpioevent.h"

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <format>

void GPIOPin::start(int pinNo,
					int chipNo)
{

#ifdef DEBUG
	fprintf(stderr, "Init.\n");
#endif

	_pinNo = pinNo;
	_chipNo = chipNo;

	thr = std::thread(&GPIOPin::worker, this);
}

void GPIOPin::gpioEvent(const gpiod::edge_event &event)
{
    if (eventCallback) eventCallback(event);
}

void GPIOPin::worker()
{
	std::string chipPath = std::format("/dev/gpiochip{}", _chipNo);

	// Config the pin as input and detecting falling and rising edegs
	gpiod::line_config line_cfg;
	line_cfg.add_line_settings(
		_pinNo,
		gpiod::line_settings()
			.set_direction(gpiod::line::direction::INPUT)
			.set_edge_detection(gpiod::line::edge::BOTH));

	try
	{
		gpiod::chip chip(chipPath);

		auto builder = chip.prepare_request();
		builder.set_consumer(consumername);
		builder.set_line_config(line_cfg);
		gpiod::line_request request = builder.do_request();

		running = true;
		while (running)
		{
			// blocking I/O: thread goes to sleep till an event has happened.
			bool r = request.wait_edge_events(std::chrono::milliseconds(ISR_TIMEOUT_MS));
			if (r)
			{
				gpiod::edge_event_buffer buffer;
				request.read_edge_events(buffer, 1);
				// callback
				gpioEvent(buffer.get_event(0));
			}
			else
			{
#ifdef DEBUG
				std::cerr << "Timeout" << std::endl;
#endif
			}
		}
		request.release();
		chip.close();
	}
	catch (const std::exception &e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return;
	}
}

void GPIOPin::stop()
{
	if (!running)
		return;
	running = false;
	thr.join();
}
