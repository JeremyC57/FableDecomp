# mk.sh <addr> <base> <tag> <old> <new> [flags]: single replacement variant, graded
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad
D=${AGENTDIR:-$S/w22/chunk727}
A=$1; B=$2; T=$3; O=$4; N=$5; shift 5
L=$(sed -n 's,^// Leaf: ,,p' $B | head -1)
python3 - "$B" "$D/work/${A}_$T.src" "$O" "$N" <<'PY' || exit 1
import sys
s=open(sys.argv[1]).read(); a,b=sys.argv[3],sys.argv[4]
assert a in s,'MISSING:'+a
open(sys.argv[2],'w').write(s.replace(a,b,1))
PY
bash $D/var.sh $A $D/work/${A}_$T.src $T "$L" - "$@"
