#!/bin/bash
# g.sh <addr> <src> [tag]: pp-check, grade with q.py (overlay hdr/), masked diff, log to LOG.tsv
S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh; export S
C=$S/w22/chunk726; cd /home/user/FableDecomp
A=$1; SRC=$2; TAG=${3:-x}
E=$(bash $C/pp.sh $SRC); if [ -n "$E" ]; then echo "PPFAIL $E"; exit 1; fi
L=$(sed -n 's,^// Leaf: ,,p' $SRC | head -1)
FL=$(sed -n 's,^// Flags: ,,p' $SRC | head -1)
if [ -n "$FL" ]; then FL="--flags $FL"; fi
O=$C/work/${A}_$TAG
FABLE_HDR_OVERLAY=$C/hdr timeout 120 python3 $S/tools/q.py $A $SRC "$L" $FL > $O.txt 2>&1
python3 $S/w22/agent724b/dq.py $O.txt 2 > $O.d
G=$(grep -oE "(RELOCATION_MATCH|MATCH|DIFFER\([0-9]+v[0-9]+\)|SRC_FAIL|[A-Z_]+FAIL)" $O.txt | head -1); [ -z "$G" ] && G=TIMEOUT
N=$(tail -1 $O.d | awk '{print $2}')
echo -e "$A\t$TAG\t$G\tdl=$N" | tee -a $C/LOG.tsv
