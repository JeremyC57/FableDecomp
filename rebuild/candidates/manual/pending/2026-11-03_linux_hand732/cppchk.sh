#!/bin/bash
# cppchk.sh <src>: gcc preprocessor pass over the mirror; prints project #error / missing-include lines only
S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; D=$S/w22/chunk732
cpp -D_MSC_VER=1310 -D_M_IX86 -D_WIN32 -DWIN32 -I$D/mirror -I$D/mirror/stlport -I$S/wine/vc71rtm/include $1 -o /dev/null 2>&1 | grep -i "error" | grep -v "wine/vc71rtm/include" | head
