#!/bin/bash
# sites.sh <obj> : per push_back site, I = inline copy ctor, C = _Construct call
objdump -dr "$1" | awk '/<\?Init@CQ_NewOakValeIntroScript/{f=1} f&&/DISP32/{print $3}' | awk '/ABV0@@Z/{s=s "I"} /_Construct/{s=s "C"} END{print s, length(s)}'
