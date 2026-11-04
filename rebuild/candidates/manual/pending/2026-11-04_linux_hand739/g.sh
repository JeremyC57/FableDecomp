#!/bin/bash
# g.sh <addr> <src> <tag> : include check, grade on the full mirror with q.py, masked diff count, log to LOG.tsv
export S=${S}; source $S/wine/env.sh >/dev/null 2>&1; export S
cd /home/user/FableDecomp
D=$S/w22/chunk739; MIR=${MIR:-$D/mirror}
A=$1; SRC=$2; TAG=$3; shift 3
L=$(sed -n 's,^// Leaf: ,,p' $SRC | head -1)
FL=$(sed -n 's,^// Flags: ,,p' $SRC | head -1)
mkdir -p $D/work
M=$(python3 $S/w22/agent724b/incres.py $SRC $MIR $S/wine/vc71rtm/include | grep MISSING)
if [ -n "$M" ]; then echo "$M"; echo -e "$A\t$TAG\tMISSING_INCLUDE" >> $D/LOG.tsv; exit 1; fi
E=$(gcc -E -x c++ -D_MSC_VER=1310 -D_M_IX86=600 -D_WIN32 -I $MIR -I $S/wine/vc71rtm/include $SRC -o /dev/null 2>&1 | grep -E "#error|fatal error" | head -3)
if [ -n "$E" ]; then echo "PPFAIL $E"; echo -e "$A\t$TAG\tPPFAIL" >> $D/LOG.tsv; exit 1; fi
if [ "$FL" = "none" ]; then FL=""; fi
if [ -n "$FL" ]; then FLA="--flags $FL"; else FLA=""; fi
FABLE_HDR_OVERLAY=$MIR timeout 120 python3 $S/tools/q.py $A $SRC "$L" $FLA > $D/work/${A}_$TAG.txt 2>&1
python3 $D/dq.py $D/work/${A}_$TAG.txt 2 > $D/work/${A}_$TAG.d
G=$(grep -oE "(RELOCATION_MATCH|MATCH|DIFFER\([0-9]+v[0-9]+\)|SRC_FAIL|[A-Z_]+FAIL)" $D/work/${A}_$TAG.txt | head -1); N=$(tail -1 $D/work/${A}_$TAG.d | awk '{print $2}')
[ -z "$G" ] && G=TIMEOUT_OR_ERR
echo -e "$A\t$TAG\t$G\tdl=$N\t$(date +%H:%M)" | tee -a $D/LOG.tsv
