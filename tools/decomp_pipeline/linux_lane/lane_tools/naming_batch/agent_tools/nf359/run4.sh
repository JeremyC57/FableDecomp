#!/bin/bash
# run4.sh: NF359 emitter census (HEAD objects of two local-model rows) + pk_all --leaf on the census / precedent HEAD objects
S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; export S; source $S/wine/env.sh >/dev/null 2>&1; export S; unset TMPDIR TMP
A=$S/w22/agentNF359; T=$A/tools; W=$A/work
timeout 900 python3 $T/nf.py hgrade h4 00749a70
timeout 900 python3 $T/nf.py hgrade h4 00455f3c
timeout 900 bash $T/pk2.sh H_00749810 00749810 $W/hobj/00749810.obj '??0CTCNoise@@QAE@AAVCThing@@@Z'
timeout 900 bash $T/pk2.sh H_004317d0 004317d0 $W/hobj/004317d0.obj '??1CUIMiscThingsDef@NUISystem@@UAE@XZ'
echo RUN4_DONE
