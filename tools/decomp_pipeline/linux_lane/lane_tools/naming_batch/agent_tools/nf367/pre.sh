#!/bin/bash
# pre.sh <src.cpp>: clang MSVC-target syntax precheck (repo include, VC7.1 headers); prints errors only. NOWCHAR=1 adds -Xclang -fno-wchar.
S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; A=$S/w22/agentNF367
XW=; [ -n "$NOWCHAR" ] && XW="-Xclang -fno-wchar"
cd $A/work; timeout 120 clang++ -target i686-pc-windows-msvc -fms-extensions -fms-compatibility -fms-compatibility-version=13.10 $XW -fsyntax-only -w -I/home/user/FableDecomp/rebuild/include -isystem $S/wine/vc71/include "$1" 2>&1 | grep -E "error|fatal" | head -${N:-8}; true
