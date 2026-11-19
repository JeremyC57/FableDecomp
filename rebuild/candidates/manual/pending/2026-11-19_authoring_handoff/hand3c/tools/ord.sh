#!/bin/bash
cd /home/user/FableDecomp
ORD=1 LO=${LO:-9893a0} HI=${HI:-98942a} python3 $W/tools/side.py "$1" "$2" x | tail -2
