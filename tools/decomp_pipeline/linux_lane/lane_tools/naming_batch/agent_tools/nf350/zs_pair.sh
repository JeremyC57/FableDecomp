#!/bin/bash
# zs_pair.sh <addr>: D1 cl /Zs of the HEAD source and of rows/<addr>.cpp with the catalog flags (NF305 Q2 pair). Background, timeout 900 each.
A=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF350
S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad
source $S/wine/env.sh >/dev/null 2>&1; export S; unset TMPDIR TMP
a=$1
timeout 900 bash $A/tools/zs_head.sh $a src >> $A/work/zs_chain.log 2>&1
fl=$(python3 -c "import sys;sys.path.insert(0,'$A/tools');import nf;from pathlib import Path;print(' '.join(nf.flags_of('$a', Path('$A/rows/$a.cpp').read_text())))")
cd $A/work && timeout 900 python3 $A/tools/nf.py zs new_$a $A/rows/$a.cpp src $fl >> $A/work/zs_chain.log 2>&1
echo "ZS_PAIR_DONE $a" >> $A/work/zs_chain.log
