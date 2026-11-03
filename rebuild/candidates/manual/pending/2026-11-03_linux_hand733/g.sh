#!/bin/bash
# g.sh <addr> <src> <tag> : grade on the full mirror, log
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh >/dev/null 2>&1; export S
cd /home/user/FableDecomp
D=$S/w22/chunk733
A=$1; SRC=$2; TAG=$3; shift 3
L=$(sed -n 's,^// Leaf: ,,p' $SRC | head -1)
FL=$(sed -n 's,^// Flags: ,,p' $SRC | head -1)
mkdir -p $D/work
M=$(python3 $S/w22/agent724b/incres.py $SRC $D/mirror $S/wine/vc71rtm/include | grep MISSING)
if [ -n "$M" ]; then echo "$M"; echo -e "$A\t$TAG\tMISSING_INCLUDE" >> $D/LOG.tsv; exit 1; fi
if [ "$FL" = "none" ]; then FL=""; fi
if [ -n "$FL" ]; then FLA="--flags $FL"; else FLA=""; fi
FABLE_HDR_OVERLAY=$D/mirror timeout 120 python3 $S/tools/q.py $A $SRC "$L" $FLA > $D/work/${A}_$TAG.txt 2>&1
python3 $S/w22/agent724b/dq.py $D/work/${A}_$TAG.txt 2 > $D/work/${A}_$TAG.d
G=$(grep -oE "(RELOCATION_MATCH|MATCH|DIFFER\([0-9]+v[0-9]+\)|SRC_FAIL|[A-Z_]+FAIL)" $D/work/${A}_$TAG.txt | head -1); N=$(tail -1 $D/work/${A}_$TAG.d | awk '{print $2}')
[ -z "$G" ] && G=TIMEOUT_OR_ERR
echo -e "$A\t$TAG\t$G\tdl=$N\t$(date +%H:%M)" | tee -a $D/LOG.tsv
