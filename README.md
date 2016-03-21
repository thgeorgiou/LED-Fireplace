# LED-Fireplace
A little LED panel creating atmosphere

Using a [Teensy 3.1](https://www.pjrc.com/teensy/) and [a 32x16 Pixel Matrix](https://www.adafruit.com/product/420)
I created a small LED fireplace which provides some nice ambience to the room at dark. The code is heavily based on
the [Aurora](https://github.com/pixelmatix/aurora), a more complete art display project.

## Usage
The code depands on the [SmartMatrix3]() and [FastLED](https://github.com/FastLED/FastLED) libraries. Both are available
through the Arduino Library manager.

## Features/TODO
Currently the fire is rendered and the display can be turned on/off by touching a bolt on the frame. In the future I want
to add a clock and make it configurable (set time, brightness, auto turn on/off) through serial to minize the amount of
input channels needed.

# License
My small code snippet is available in the Public Domain. All libraries are copyrighted by their respective authors.
For more info check the LICENSE file.