#!/bin/bash
# g.sh <addr> <src.cpp> <tag> [flags...]: incres check, grade on the full mirror, log
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh; export S
cd /home/user/FableDecomp
D=$S/w22/chunk730
A=$1; B=$2; T=$3; shift 3
L=$(sed -n 's,^// Leaf: ,,p' $B | head -1)
M=$(python3 $D/incres.py $B $D/mirror $(dirname $B) $FABLE_VC71/include | grep MISSING)
if [ -n "$M" ]; then echo "$M"; echo -e "$A\t$T\tMISSING_INCLUDE" >> $D/LOG.tsv; exit 1; fi
E=$(bash $D/cppchk.sh $B | grep "#error")
if [ -n "$E" ]; then echo "$E" | head -3; echo -e "$A\t$T\tCPP_ERROR" >> $D/LOG.tsv; exit 1; fi
cp $B $D/work/${A}_$T.cpp
if [ $# -gt 0 ]; then FL="--flags $*"; else FL=""; fi
FABLE_HDR_OVERLAY=$D/mirror timeout 120 python3 $S/tools/q.py $A $D/work/${A}_$T.cpp "$L" $FL > $D/work/${A}_$T.txt 2>&1
python3 $D/dq.py $D/work/${A}_$T.txt 2 > $D/work/${A}_$T.d
G=$(grep -oE "(RELOCATION_MATCH|MATCH|DIFFER\([0-9]+v[0-9]+\)|SRC_FAIL|[A-Z_]+FAIL)" $D/work/${A}_$T.txt | head -1); N=$(tail -1 $D/work/${A}_$T.d | awk '{print $2}')
[ -z "$G" ] && G=TIMEOUT_OR_ERR
echo -e "$A\t$T\t$G\tdl=$N" | tee -a $D/LOG.tsv
