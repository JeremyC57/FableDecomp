#!/bin/bash
# hg.sh <tag> <addr...>: nf.py hgrade (HEAD objects only -> work/hobj/<a>.obj), log work/gr_<tag>.log
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh >/dev/null 2>&1; export S; unset TMPDIR TMP
A=$S/w22/agentNF336; cd $A/work
python3 $A/tools/nf.py hgrade "$@"
