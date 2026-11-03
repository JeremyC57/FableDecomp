#!/bin/bash
# g.sh <addr> <src> <tag> [flags]: compile check (cc.sh) then q.py grade + masked diff + call alignment; logs to LOG.tsv
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh; export S
cd /home/user/FableDecomp; C=$S/w22/chunk729
A=$1; SRC=$2; T=$3; shift 3
cp $SRC $C/cl/$T.cpp
R=$(timeout 110 $C/cc.sh $C/cl/$T.cpp "$@" | tail -5)
if ! echo "$R" | grep -q "^obj"; then echo "$R"; echo -e "$A\t$T\tSRC_FAIL\t-" >> $C/LOG.tsv; echo SRC_FAIL; exit 1; fi
L=$(sed -n 's,^// Leaf: ,,p' $SRC | head -1)
if [ $# -gt 0 ]; then FL="--flags $*"; else FL=""; fi
FABLE_HDR_OVERLAY=$C/hdr timeout 120 python3 $S/tools/q.py $A $SRC "$L" $FL > $C/work/$T.txt 2>&1
python3 $S/w22/agent724b/dq.py $C/work/$T.txt 1 > $C/work/$T.d
G=$(grep -oE "(RELOCATION_MATCH|MATCH|DIFFER\([0-9]+v[0-9]+\)|SRC_FAIL|[A-Z_]+FAIL)" $C/work/$T.txt | head -1); N=$(tail -1 $C/work/$T.d | awk '{print $2}')
case $A in 005ff190) LN=0x1050;; 00603000) LN=0x2563;; esac
python3 $C/calls.py $A $LN $C/cl/$T.obj $(echo "$L" | cut -c1-20 | sed 's/^?//') > $C/work/$T.calls 2>&1
echo -e "$A\t$T\t$G\tdl=$N\t$(tail -1 $C/work/$T.calls)" | tee -a $C/LOG.tsv
