#!/bin/bash
# fwn.sh <thunk addrs...>: resolve FableWin thunk -> name
W=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w24/hand4f
for a in "$@"; do a=${a#0x}; ins=$(python3 $W/ddis.py fw $a 5 | head -1); t=$(echo "$ins" | awk '{print $4}'); if echo "$ins" | grep -q jmp; then t=$(printf "%08x" $t); else t=$a; fi; echo "$a -> $t $(grep -P "^$t\t" /home/user/FableDecomp/ghidra_out/fablewin_pdb_names.tsv | cut -f2 | cut -c1-160)"; done
