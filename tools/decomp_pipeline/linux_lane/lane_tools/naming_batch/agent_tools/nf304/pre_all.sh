#!/bin/bash
A=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF304; W=$A/work
src(){ python3 -c "import sys; sys.path.insert(0,'$A/tools'); import nf; print(nf.src_of('$1'))"; }
for a in $(cat $W/chl.txt); do nw=; case $a in 00a5b710|00a5b760) nw=1;; esac
  timeout 120 python3 $A/tools/nf.py pre N_$a $A/rows/$a.cpp $nw > /dev/null; timeout 120 python3 $A/tools/nf.py pre H_$a $(src $a) $nw > /dev/null; done
for a in $(cat $W/ctl.txt); do nw=; case $a in 0044a2a0|0048f939) nw=1;; esac; timeout 120 python3 $A/tools/nf.py pre H_$a $(src $a) $nw > /dev/null; done
echo done > $W/pre_all.done
