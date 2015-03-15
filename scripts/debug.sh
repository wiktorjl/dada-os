#!/bin/sh

#qemu-system-i386 -s -S -curses -kernel build/kernel.bin

#qemu-system-i386 -s -S -display curses -kernel build/kernel.bin  
qemu -s -S -nographic -m 2047m -display curses -kernel build/kernel.bin  -serial file:serial.out -no-shutdown
