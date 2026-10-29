#!/bin/bash
# usage: run2.sh <name> [fnsym]  -- compile exp/<name>.cpp with VC7.1 /O2 and print the skeleton of Test (or fnsym)
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad
source $S/wine/env.sh; export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad
D=$S/w22/agent572f/exp; f=$1; fn=${2:-?Test@@YAXH@Z}
cd $D && timeout 120 wine $FABLE_VC71/bin/cl.exe /nologo /c /O2 /Oy /W3 /Fo$f.obj $f.cpp > $f.log 2>&1; grep -i "error" $f.log | head
python3 $S/w22/agent572f/tools/skel.py $D/$f.obj "$fn"
