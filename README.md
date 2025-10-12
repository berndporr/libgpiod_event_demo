# C++ libgpiod event coding example for the Raspberry PI

This code demonstrates how a rising or falling edge at a
GPIO pin wakes up a thread which then runs a callback.
This then allows the communication between classes where a class subscribes
to a callback interface. `std::function` and lambda
functions are used to glue together publishers and subscribers.

## Prerequisites

You need Debian `trixie` on your Raspberry PI.

You need to install the libgpiod v2 development package:

```
apt-get install libgpiod-dev
```

## How to compile

The build system is `cmake`:

```
cmake .
make
make install
```

## How to use it?

Please check out the example in the subdir example: `gpio_printer`.
This detects rising and falling edges on GPIO pin 27. 
It registers the method `hasEvent` as a callback in the class `EventPrinter`.
To play with it just
connect a push button to GPIO27 with a pullup resistor.

![alt tag](example/wiring.jpg)

Press the button and you should see:
```
Press any key to stop.
Falling
Rising!
Falling
Rising!
Falling
```

## Credit

Bernd Porr
