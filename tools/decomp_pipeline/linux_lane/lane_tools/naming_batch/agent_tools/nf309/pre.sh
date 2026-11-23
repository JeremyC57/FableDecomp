#!/bin/bash
# pre.sh <src.cpp>: clang MSVC-target syntax precheck (agent overlay first, repo include, VC7.1 headers); prints errors only.
# NOWCHAR=1 adds -Xclang -fno-wchar (VC7.1 default /Zc:wchar_t-). OVL=<dir> overrides the overlay dir (default the agent hdr/).
# NF309: also prints CLANG_RC=<rc> when clang exits nonzero (a timeout = 124) without an error line, so a silent run is a real pass.
S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad; A=$S/w22/agentNF309
XW=; [ -n "$NOWCHAR" ] && XW="-Xclang -fno-wchar"; OV=${OVL:-$A/hdr}
cd $A/work; out=$(timeout 60 clang++ -target i686-pc-windows-msvc -fms-extensions -fms-compatibility -fms-compatibility-version=13.10 $XW -fsyntax-only -w -I$OV -I/home/user/FableDecomp/rebuild/include -isystem $S/wine/vc71/include "$1" 2>&1); rc=$?
e=$(echo "$out" | grep -E "error|fatal" | head -${N:-8}); [ -n "$e" ] && echo "$e"; [ $rc -ne 0 ] && [ -z "$e" ] && echo "CLANG_RC=$rc"; true
