#!/bin/bash
# g.sh <addr> <leaf> <src...> [-- flags]: grade each source, save .out, print normalised summary line
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad
source $S/wine/env.sh >/dev/null 2>&1
a=$1; leaf=$2; shift 2
srcs=(); flags=()
while [ $# -gt 0 ]; do if [ "$1" = "--" ]; then shift; flags=("$@"); break; fi; srcs+=("$(readlink -f "$1")"); shift; done
cd /home/user/FableDecomp
for s in "${srcs[@]}"; do
  o=${s%.cpp}.out
  if [ ${#flags[@]} -gt 0 ]; then timeout 900 python3 $S/tools/q.py $a $s "$leaf" --flags "${flags[@]}" > $o 2>&1; else timeout 900 python3 $S/tools/q.py $a $s "$leaf" > $o 2>&1; fi
  if grep -q SRC_FAIL $o; then echo "$(basename $s) SRC_FAIL"; grep -m3 "error" $o; else echo -n "$(basename $s) "; python3 $S/w22/auth1/nd.py $o 2>/dev/null | head -1 | sed 's/.*\(DIFFER\|RELOCATION_MATCH\|MATCH\)/\1/'; fi
done
