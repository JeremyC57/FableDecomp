#!/usr/bin/env bash
# chain361.sh <tag> <addr> <kind>:<src> ...: per row, one pass.py grade per variant in order (each its own `timeout 900` launch line),
# kinds: H (HEAD object work/obj/H_<a>.obj), N (new object work/obj/<a>.obj), or any other word w (variant object work/obj/<a>_<w>.obj).
# Rows compile one at a time at the one path work/one/<a>/ so ?A0x hashes agree. Launch the chain itself in the background.
A=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF365
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh; export S; unset TMPDIR TMP
tag=$1; a=$2; shift 2
for spec in "$@"; do
  k=${spec%%:*}; src=${spec#*:}
  if [ "$k" = H ]; then
    timeout 900 python3 $A/tools/pass.py $tag H $a=$src
  elif [ "$k" = N ]; then
    timeout 900 python3 $A/tools/pass.py $tag N $a=$src
  else
    timeout 900 python3 $A/tools/pass.py $tag N $a@$k=$src
  fi
done
echo "CHAIN_DONE $a" >> $A/work/pass_$tag.log
