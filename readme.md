AVR8 Simulator
===============

A simulator library for AVR8 microcontroller. It only supports non-XMEGA instructions sets. The library does not support the MCU in-built peripherals, though it can be easily extended. This effort might be pursued in the future.
This simulator needs data bytes to execute instructions, and data bytes comes from hex file. So if you are using the simulator program without a hex file, u need to manually write data bytes in an array, u may call the array as flashMemory.
However, we can use Atmel Stuidio 7.0 to generate a Hex file and use it. This way, you need to use Hex Parser program to combine with this simulator to execute assembly instructions. The full combined program is [here](https://github.com/Bennyaw/HexParse.git).



