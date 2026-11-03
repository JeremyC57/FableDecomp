#!/bin/bash
# pp.sh <src>: gcc preprocess check with the overlay first (catches #error / missing include before cl spins)
S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; C=$S/w22/chunk726
cd /home/user/FableDecomp
gcc -E -x c++ -D_MSC_VER=1310 -D_M_IX86=600 -D_WIN32 -I $C/hdr -I rebuild/include -I $S/wine/vc71rtm/include $1 -o /dev/null 2>&1 | grep -E "error|fatal" | head -5
