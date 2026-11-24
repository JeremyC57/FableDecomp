#!/usr/bin/env bash
# run362.sh <tag> <addr> [variant...]: sequential one-path compiles of one row (pass.py: HEAD as H, then each rows/var/<addr>_<variant>.cpp
# as <addr>@<variant>, all at work/one/<addr>/<addr>.cpp), then both checkers (pkrun352.py) on every object into work/pk/<tag>/.
# Launch in the background; each compile has `timeout 900` on its own line.
A=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF362
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad
source $S/wine/env.sh >/dev/null; export S; unset TMPDIR TMP
export FABLE_ROOT=/home/user/FableDecomp
tag=$1; a=$2; shift 2
cd $A/work
sp=$(python3 -c "import sys; sys.path.insert(0,'$A/tools'); import nf; print(nf.src_of('$a'))")
timeout 900 python3 $A/tools/pass.py $tag H "$a=$sp"
specs=""
for v in "$@"; do
  timeout 900 python3 $A/tools/pass.py $tag N "$a@$v=$A/rows/var/${a}_$v.cpp"
done
lab="H=$A/work/obj/H_$a.obj"
for v in "$@"; do lab="$lab $v=$A/work/obj/${a}_$v.obj"; done
timeout 900 python3 $A/tools/pkrun352.py $A/work/pk/$tag $a $lab
echo "RUN_DONE $tag $a $*" >> $A/work/run_done.log
