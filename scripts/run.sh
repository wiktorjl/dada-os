#!/bin/sh

qemu -hda disk.img -nographic -m 2047m -display curses -s -kernel build/kernel.bin  -serial file:serial.out -d int -no-shutdown

#-debugcon stdio
