#!/bin/bash
# sib.sh <addr> <src> <leaf>: incres check then q.py under the overlay
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh; export S; cd /home/user/FableDecomp; C=$S/w22/chunk729
M=$(python3 $S/w22/agent724b/incres.py $2 $C/hdr rebuild/include $FABLE_VC71/include | grep -c MISSING)
if [ "$M" != "0" ]; then echo -e "$1\tINCRES_MISSING"; exit; fi
G=$(FABLE_HDR_OVERLAY=$C/hdr timeout 110 python3 $S/tools/q.py $1 $2 "$3" 2>&1 | grep -oE "RELOCATION_MATCH|MATCH|DIFFER\([0-9v]+\)|SRC_FAIL" | head -1)
echo -e "$1\t${G:-TIMEOUT}"
