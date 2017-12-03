#!/bin/bash
CFILE=$1
msp430-gcc -mmcu=msp430g2553 -Wall -pedantic $CFILE -o $CFILE.elf
