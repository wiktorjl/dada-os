#!/bin/sh

qemu -nographic -m 32m -display curses -s -kernel build/kernel.bin  -serial file:serial.out -d int -no-shutdown

#-debugcon stdio
