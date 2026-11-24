#!/usr/bin/env bash
# runprobe357.sh <tag> <addr> <kinds...>: one row at ONE path (work/one/<addr>/), one `timeout 900` pass.py grade per kind.
#   H   repo HEAD source, HEAD tree work/tree (full copy of rebuild/include)       -> work/obj/H_<a>.obj
#   N   rows/<a>.cpp, new tree work/treeN (work/tree + hdr/engine)                -> work/obj/<a>.obj
#   Pn  work/var/<a>_Pn.cpp, new tree                                              -> work/obj/<a>_Pn.obj
#   Qn  work/var/<a>_Qn.cpp, HEAD tree (controls)                                  -> work/obj/<a>_Qn.obj
# Launch in the background.
A=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF357
tag=$1; a=$2; shift 2
src=$(python3 -c "import sys; sys.path.insert(0,'$A/tools'); import nf; print(nf.src_of('$a'))")
for k in "$@"; do
  case $k in H|Q*) T=$A/work/tree ;; *) T=$A/work/treeN ;; esac
  case $k in H) spec="$a=$src"; kk=H ;; N) spec="$a=$A/rows/$a.cpp"; kk=N ;; *) spec="$a@$k=$A/work/var/${a}_$k.cpp"; kk=N ;; esac
  FABLE_HDR_OVERLAY=$T timeout 900 python3 $A/tools/pass.py "$tag$k" $kk "$spec"
done
echo RUNPROBE_DONE $tag $a "$@" >> $A/work/runprobe_done.log
