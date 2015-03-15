#!/bin/sh

#qemu -display curses -m 2047m -kernel build/kernel.bin  
qemu -nographic -m 2047m -display curses -s -kernel build/kernel.bin  -serial file:serial.out -no-shutdown

#-debugcon stdio
