#!/bin/bash
# unddiff.sh <head.obj> <new.obj>: undefined (sec 0) and defined external symbols, HEAD v new (lines prefixed - / +).
und(){ objdump -t "$1" | grep "(sec  0)" | grep "(scl   2)" | awk '{print "UND "$NF}' | sort -u; }
def(){ objdump -t "$1" | grep -v "(sec  0)" | grep "(scl   2)" | awk '{print "DEF "$NF}' | sort -u; }
diff <( (und "$1"; def "$1") | sort -u) <( (und "$2"; def "$2") | sort -u) | grep '^[<>]' | sed 's/^</-/;s/^>/+/'
echo "unddiff done"
