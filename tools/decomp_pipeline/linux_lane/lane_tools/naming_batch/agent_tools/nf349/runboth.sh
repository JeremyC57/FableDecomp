#!/usr/bin/env bash
# runboth.sh <tag> <addr file>: per row, HEAD then new, one `timeout 900` pass.py grade each, at ONE path per row (work/one/<addr>/);
# objects work/obj/H_<addr>.obj and work/obj/<addr>.obj. Logs work/pass_<tag>H.log / pass_<tag>N.log. Run the whole script in the background.
A=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF349
while read -r a; do
  [ -n "$a" ] || continue
  src=$(python3 -c "import sys; sys.path.insert(0,'$A/tools'); import nf; print(nf.src_of('$a'))")
  timeout 900 python3 $A/tools/pass.py "${1}H" H "$a=$src"
  timeout 900 python3 $A/tools/pass.py "${1}N" N "$a=$A/rows/$a.cpp"
done < "$2"
echo RUNBOTH_DONE >> $A/work/pass_${1}N.log
