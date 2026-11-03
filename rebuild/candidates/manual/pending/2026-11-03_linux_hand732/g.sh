#!/bin/bash
# g.sh <addr> <src> <tag> [flags...]: precheck includes, grade on the full mirror, log
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh >/dev/null 2>&1; export S
cd /home/user/FableDecomp; D=$S/w22/chunk732
A=$1; SRC=$2; TG=$3; shift 3
L=$(sed -n 's,^// Leaf: ,,p' $SRC | head -1)
M=$(python3 $S/w22/agent724b/incres.py $SRC $D/mirror $D/mirror/stlport $S/wine/vc71rtm/include 2>&1 | grep MISSING | grep -v "MISSING \(windows.h\|new\|stddef.h\|string.h\|stdlib.h\|math.h\|memory\|float.h\|cstdio\|cstring\|cstdlib\)")
if [ -n "$M" ]; then echo "PRECHECK: $M"; fi
if [ $# -gt 0 ]; then FL="--flags $*"; else FL=""; fi
cp $SRC $D/work/${A}_$TG.cpp
FABLE_HDR_OVERLAY=$D/mirror timeout 120 python3 $S/tools/q.py $A $SRC "$L" $FL > $D/work/${A}_$TG.txt 2>&1
python3 $D/dq.py $D/work/${A}_$TG.txt 2 > $D/work/${A}_$TG.d
G=$(grep -oE "(RELOCATION_MATCH|MATCH|DIFFER\([0-9]+v[0-9]+\)|SRC_FAIL|[A-Z_]+FAIL)" $D/work/${A}_$TG.txt | head -1); N=$(tail -1 $D/work/${A}_$TG.d | awk '{print $2}')
[ -z "$G" ] && G="NOGRADE($(head -c 200 $D/work/${A}_$TG.txt | tr '\n' ' '))"
echo -e "$A\t$TG\t$G\tdl=$N\t$*" | tee -a $D/LOG.tsv
