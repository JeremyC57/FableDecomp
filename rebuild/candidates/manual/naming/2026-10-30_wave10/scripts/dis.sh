#!/bin/bash
# dis.sh <hexaddr> [len]: disassemble retail Fable.exe
S=/tmp/claude-0/-home-user-FableDecomp/a013f3ec-d440-5b47-87a1-ed975cb80dfb/scratchpad
a=$((16#${1#0x})); n=$((${2:-64}))
objdump -d -M intel --no-show-raw-insn --start-address=$a --stop-address=$((a+n)) "$S/dl/game/Fable The Lost Chapters/Fable.exe" | tail -n +8
