#!/bin/bash
# mutant.sh <addr> <row.cpp> <leaf> <sed-expr> <test.cpp> [flags...]: compile a sed-mutated copy of the row (overlay $W/hdr), link the
# behaviour test against it; the test must FAIL. Appends to $W/evidence/mutants.txt.
S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; export S
source $S/wine/env.sh >/dev/null 2>&1; export S; unset TMPDIR TMP
W=$S/w22/agentNF445; cd /home/user/FableDecomp; export FABLE_HDR_OVERLAY=$W/hdr
a=$1; src=$2; leaf=$3; ex=$4; t=$5; shift 5; fl=(); [ $# -gt 0 ] && fl=(--flags "$@")
m=$W/work/mut_$a.cpp; sed "$ex" "$src" > $m
cmp -s $m $src && { echo "$a MUTANT_NOOP $ex" | tee -a $W/evidence/mutants.txt; exit 0; }
g=$(python3 $S/tools/q.py $a $m "$leaf" "${fl[@]}" 2>&1 | grep -m1 "^$a " | cut -c1-60)
r=$(python3 $S/tools/vt.py $a $t $S/wine/q/$a/$a.obj 2>&1 | tail -1)
echo "$a mutant [$ex] grade=[$g] test=[$r]" | tee -a $W/evidence/mutants.txt
