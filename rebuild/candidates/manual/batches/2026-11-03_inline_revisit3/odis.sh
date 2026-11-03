#!/bin/bash
# odis.sh addr : our built function disassembly from work/<addr>/<addr>.obj (leaf substring optional)
I=${S}/pragma3
objdump -d -r -M intel --no-show-raw-insn $I/work/$1/${3:-plain}/$1.obj | awk -v L="$2" '/^[0-9a-f]+ <.*>:/{p=(index($0,L)>0)} p'
