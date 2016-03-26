# LED-Fireplace
![Photo of the LED Fireplace](http://thgeorgiou.com/images/2016-03-25-LED-Fireplace/header.jpg)

A little LED panel creating atmosphere ([Blog post](http://thgeorgiou.com/posts/2016-03-25-LED-Fireplace/))

Using a [Teensy 3.1](https://www.pjrc.com/teensy/) and [a 32x16 Pixel Matrix](https://www.adafruit.com/product/420)
I created a small LED fireplace which provides some nice ambience to the room at dark. The code is heavily based on
the [Aurora](https://github.com/pixelmatix/aurora), a more complete art display project. 

## Usage
The code depands on the [SmartMatrix3](https://github.com/pixelmatix/SmartMatrix) and [FastLED](https://github.com/FastLED/FastLED) libraries. Both are available through the Arduino Library manager.

## Features/TODO
Currently the fire is rendered and the display can be turned on/off by touching a bolt on the frame. Touching the bolt cycles
through three modes, off, on, and on w/ clock. The clock is apperantly programmed when flashing the board so it works fine
without a way to set it but I still want to implement a serial interface to set the clock and maybe auto turn-on/off on schedule.

# License
My small code snippet is available in the Public Domain. All libraries are copyrighted by their respective authors.
For more info check the LICENSE file.
