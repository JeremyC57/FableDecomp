#!/bin/bash
# pdbq.sh <mangled>...: is the exact symbol in the Ego_r / FableWin PDB name sets (counts per file)
G=/home/user/FableDecomp/ghidra_out
for s in "$@"; do e=$(grep -c -F -- "$s" $G/egor_pdb_names.tsv); f=$(grep -c -F -- "$s" $G/fablewin_pdb_names.tsv); echo "egor=$e fablewin=$f $s"; done
