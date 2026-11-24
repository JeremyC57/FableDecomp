#!/bin/bash
# zs_head.sh <addr> <src|test>: D1 cl /Zs of the HEAD source / test with the catalog flags (nf.py zs). Run in background with timeout 900.
A=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF343
a=$1; k=$2
f=$(python3 -c "import sys;sys.path.insert(0,'$A/tools');import nf;print(nf.src_of('$a') if '$k'=='src' else nf.test_of('$a'))")
fl=$(python3 -c "import sys;sys.path.insert(0,'$A/tools');import nf;from pathlib import Path;print(' '.join(nf.flags_of('$a', Path('$f').read_text())))")
cd $A/work && python3 $A/tools/nf.py zs ${k}_$a "$f" $k $fl
