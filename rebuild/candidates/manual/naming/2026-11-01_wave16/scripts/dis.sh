#!/bin/bash
# dis.sh <hexaddr> [len]: disassemble retail Fable.exe
S=${S:?set S to the scratch lane root}
a=$((16#${1#0x})); n=$((${2:-64}))
objdump -d -M intel --no-show-raw-insn --start-address=$a --stop-address=$((a+n)) "$S/dl/game/Fable The Lost Chapters/Fable.exe" | tail -n +8
