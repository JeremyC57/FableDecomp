#!/bin/bash
# g5.sh: entries 223/224, binder 0x00405650 with the hdr/fable_ime.h opt-in overlay: precheck (clang, hdr/ first), grade H+N with overlay
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh >/dev/null 2>&1; export S; unset TMPDIR TMP
A=$S/w22/agentNF338; cd /home/user/FableDecomp
timeout 300 python3 $A/tools/nf.py pre N_00405650 $A/rows/00405650.cpp > $A/work/g5b_pre.out 2>&1
FABLE_HDR_OVERLAY=$A/hdr timeout 900 python3 $A/tools/nf.py grade g5b 00405650
echo done > $A/work/g5b.done
