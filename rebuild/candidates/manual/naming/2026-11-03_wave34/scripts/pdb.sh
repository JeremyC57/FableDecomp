#!/bin/bash
cd /home/user/FableDecomp; for s in "$@"; do echo "-- $s"; grep -F -- "$s" ghidra_out/egor_pdb_names.tsv | head -3 | sed 's/^/  egor /'; grep -F -- "$s" ghidra_out/fablewin_pdb_names.tsv | head -3 | sed 's/^/  fw   /'; done
