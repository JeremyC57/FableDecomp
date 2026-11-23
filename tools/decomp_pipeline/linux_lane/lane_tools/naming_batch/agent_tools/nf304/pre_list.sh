#!/bin/bash
# pre_list.sh <rows changed file> <controls file> <donefile>: clang prechecks (nf.py pre) of every changed row's new + HEAD source
# (+ rows/<a>_test.cpp when present) and every control's HEAD source, all into work/pre.log; NOWCHAR for addrs in work/nowchar.txt
A=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF304; W=$A/work
src(){ python3 -c "import sys; sys.path.insert(0,'$A/tools'); import nf; print(nf.src_of('$1'))"; }
nw(){ grep -qx "$1" $W/nowchar.txt 2>/dev/null && echo 1; }
for a in $(cat $1); do
  timeout 120 python3 $A/tools/nf.py pre N_$a $A/rows/$a.cpp $(nw $a) > /dev/null
  timeout 120 python3 $A/tools/nf.py pre H_$a $(src $a) $(nw $a) > /dev/null
  [ -f $A/rows/${a}_test.cpp ] && timeout 120 python3 $A/tools/nf.py pre NT_$a $A/rows/${a}_test.cpp > /dev/null
done
for a in $(cat $2); do timeout 120 python3 $A/tools/nf.py pre H_$a $(src $a) $(nw $a) > /dev/null; done
echo done > $3
