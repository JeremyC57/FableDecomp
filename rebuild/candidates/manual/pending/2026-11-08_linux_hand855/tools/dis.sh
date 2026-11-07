#!/bin/bash
# dis.sh <hexaddr> <len>  -> intel disassembly of Fable.exe
s=$((16#$1)); e=$((s+$2))
objdump -d -M intel --no-show-raw-insn --start-address=$s --stop-address=$e "$S/dl/game/Fable The Lost Chapters/Fable.exe" | tail -n +8
