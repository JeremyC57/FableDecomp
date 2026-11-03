#!/bin/bash
# g.sh <agentdir> <addr> <src> <leaf> <note> [-- flags]
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad
source $S/wine/env.sh; export S; cd /home/user/FableDecomp
D=$1; A=$2; SRC=$3; LEAF=$4; shift 4; NOTE="$1"; shift
FL=""; if [ "$1" == "--" ]; then shift; FL="--flags $*"; fi
OUT=$(FABLE_HDR_OVERLAY=$D/hdr timeout 200 python3 $S/tools/q.py $A $SRC "$LEAF" $FL 2>&1)
ST=$(echo "$OUT" | head -1 | awk '{print $3}')
RB=$(echo "$OUT" | grep -o "^--- retail ([0-9]* bytes)" | grep -o "[0-9]*")
BB=$(echo "$OUT" | grep -o "^--- built ([0-9]* bytes)" | grep -o "[0-9]*")
echo "$OUT" > $D/last_$A.txt
# diff count of instruction lines (mnemonic+ops, addresses stripped)
python3 - "$D/last_$A.txt" > $D/diff_$A.txt <<'PY'
import sys,re,difflib
t=open(sys.argv[1]).read()
if '--- retail' not in t: sys.exit()
r=t.split('--- retail')[1].split('--- built')[0].splitlines()[1:]
b=t.split('--- built')[1].splitlines()[1:]
norm=lambda L:[re.sub(r'\[(\w+\*4) \+ X\]',r'[\1]',re.sub(r'\[0\]','[X]',re.sub(r'0x[0-9a-f]{5,8}','X',l[33:]))).strip() for l in L if l.strip()]
R,B=norm(r),norm(b)
d=[l for l in difflib.unified_diff(R,B,lineterm='',n=1)]
print(sum(1 for l in d if l[:1] in '+-' and l[:3] not in ('+++','---')))
print('\n'.join(d))
PY
ND=$(head -1 $D/diff_$A.txt)
N=$(grep -c "^$A	" $D/LOG.tsv 2>/dev/null); N=$((N+1))
echo -e "$A\t$N\t$ST\t${BB:-?}v${RB:-?}\tdl=${ND:-?}\t$NOTE $FL" >> $D/LOG.tsv
echo "$A attempt $N: $ST ${BB:-?}v${RB:-?} difflines=${ND:-?}"
