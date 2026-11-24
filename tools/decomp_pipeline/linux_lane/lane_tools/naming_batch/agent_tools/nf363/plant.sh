#!/bin/bash
# plant.sh <pk_gate dir> <addr> <out dir>: comparator-level planted control (NF363). Copies H_/N_<addr> pk files to <out dir> and appends one
# synthetic leaf-called NEQ copy (?PlantedCopy@@QAEXXZ) to N's pk_all tsv and pvr text; pkcmp346 must then report NEWLY_NEQ / GATE FAIL and
# reachflip_coordinator must count one flip.
set -e; mkdir -p "$3"; cp "$1"/H_$2.* "$1"/N_$2.* "$3"/
printf 'NEQ\tpickany\t0042b0aa\t-\tpickany\t12\t-\ty\ty\t?PlantedCopy@@QAEXXZ\n' >> "$3"/N_$2.pkall.tsv
printf 'NEQ 0042b0aa pickany ?PlantedCopy@@QAEXXZ LEAF-REFERENCED\n' >> "$3"/N_$2.pvr.txt
