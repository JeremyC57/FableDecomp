#!/bin/bash
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; A=$S/w22/agentNF338; export NF_STUB=$A/work/stubinc
cd /home/user/FableDecomp
python3 $A/tools/equiv338.py $A/work/eq/old $A/hdr $A/work/eq/ctl $A/work/eq/includers.tsv $A/work/eq/addrs.txt > $A/work/eq/includers.out 2>&1
python3 $A/tools/equiv338.py $A/work/eq/old $A/hdr $A/work/eq/ctl $A/work/eq/all.tsv > $A/work/eq/all.out 2>&1
echo done > $A/work/eq/done
