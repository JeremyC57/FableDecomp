#!/bin/bash
# cppchk.sh <src>: gcc preprocessor pass with MSVC-ish macros; prints #error / missing-include lines
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh
D=$S/w22/chunk730
timeout 30 cpp -D_MSC_VER=1310 -D_M_IX86=600 -D_WIN32 -D__cdecl= -I$D/mirror -I$FABLE_VC71/include $1 -o /dev/null 2>&1 | grep -E "error|No such" | grep -v "Only Win32\|Only Mac" | head
