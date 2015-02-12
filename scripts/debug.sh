#!/bin/sh

#qemu-system-i386 -s -S -curses -kernel build/kernel.bin

qemu-system-i386 -s -S -curses -kernel build/kernel.bin  -serial file:serial.out -d int -no-shutdown


#-debugcon stdio
