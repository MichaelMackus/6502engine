# 6502 emulator

Simple 6502 emulator emulating some basic commodore 64 KERNAL calls

# Usage

You must first assemble your assembly file. For example, with CA65:

`cl65 examples/save.s`

Then you can run the file with:

`./engine examples/save`

The following linux command proves the program should not differ from the
saved file:

`diff HELLO  examples/save`
