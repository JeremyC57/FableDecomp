#!/bin/bash
# run1.sh: NF359 first measurement round: HEAD objects of the unit + precedent rows, then the probe compiles (sequential, one cl at a time)
S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; export S; source $S/wine/env.sh >/dev/null 2>&1; export S; unset TMPDIR TMP
A=$S/w22/agentNF359; T=$A/tools; E=$A/work/exp
timeout 900 python3 $T/nf.py hgrade h1 0045c5f6
timeout 900 python3 $T/nf.py hgrade h1 00455ef0
timeout 900 python3 $T/nf.py hgrade h1 0044c1bb
timeout 900 python3 $T/nf.py hgrade h1 0045c5da
timeout 900 python3 $T/nf.py hgrade h1 00432c40
timeout 900 python3 $T/nf.py hgrade h1 004317d0
timeout 900 python3 $T/nf.py hgrade h1 004304e0
timeout 900 python3 $T/nf.py hgrade h1 00749810
timeout 900 python3 $T/exp.py 00455ef0 $E/c1_allocform_455ef0.cpp c1_allocform_455ef0
timeout 900 python3 $T/exp.py 0044c1bb $E/d1_allocform_44c1bb.cpp d1_allocform_44c1bb
timeout 900 python3 $T/exp.py 0045c5da $E/e1_allocform_45c5da.cpp e1_allocform_45c5da
FABLE_HDR_OVERLAY=$E/ov timeout 900 python3 $T/exp.py 00455ef0 $E/c2_ool_455ef0.cpp c2_ool_455ef0
timeout 900 python3 $T/exp.py 0044c1bb $E/d2_ool_44c1bb.cpp d2_ool_44c1bb
timeout 900 python3 $T/exp.py 00455ef0 $E/c3_nf86_455ef0.cpp c3_nf86_455ef0
echo RUN1_DONE
