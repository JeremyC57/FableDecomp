#!/bin/bash
# zstest.sh (NF328) <tag> <label:file>...: D1 VC7.1 `cl /Zs /Od /W3` prechecks of TEST sources (tools/nf.py zs ... test) -> work/pre_zs.log
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh >/dev/null 2>&1; export S
A=$S/w22/agentNF328; cd $A/work; T=$1; shift
for x in "$@"; do python3 $A/tools/nf.py zs ${x%%:*} ${x#*:} test; done
echo done > $A/work/zs_$T.done
