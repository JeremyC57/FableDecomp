#!/bin/bash
# relocs.sh <obj> <leaf>: list relocation symbols used by one function
objdump -dr "$1" | awk -v L="<$2>:" 'index($0,L){f=1;next} f&&/^[0-9a-f]+ <.*>:$/{f=0} f' | grep -E "dir32|REL32|DISP32" | awk '{print $3}' | sort | uniq -c
