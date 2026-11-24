#!/usr/bin/env bash
# runprobe352.sh <tag> <addr> <kinds...>: one row at ONE path (work/one/<addr>/), one `timeout 900` pass.py grade per kind, every kind
# through ONE full include tree work/tree (a copy of rebuild/include) whose engine/CCountedPointer.h is swapped before each pass:
#   H   repo HEAD source,            HEAD header (orig/hdr_CCountedPointer)  -> work/obj/H_<a>.obj
#   Pn  work/var/<a>_Pn.cpp,          new header (hdr/engine)                -> work/obj/<a>_Pn.obj
#   N   rows/<a>.cpp,                 new header                             -> work/obj/<a>.obj
#   Q*  work/var/<a>_Q*.cpp,          HEAD header (a control)                -> work/obj/<a>_Q*.obj
#   A*  work/var/<a>_A*.cpp, new header + scratch work/hdrA/engine/NUISystem_CAction.h (DeleteData<CAction> probe)
#   I*  work/var/<a>_I*.cpp,          inline-form header work/hdrI (NF225 `template<> inline`, no noinline) -> work/obj/<a>_I*.obj
# ONE runner at a time (it swaps the tree's header). Launch in the background.
A=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF360
T=$A/work/tree; tag=$1; a=$2; shift 2
src=$(python3 -c "import sys; sys.path.insert(0,'$A/tools'); import nf; print(nf.src_of('$a'))")
for k in "$@"; do
  case $k in H|Q*) cp $A/orig/hdr_CCountedPointer/CCountedPointer.h $T/engine/CCountedPointer.h ;; I*) cp $A/work/hdrI/engine/CCountedPointer.h $T/engine/CCountedPointer.h ;; A*) cp $A/hdr/engine/CCountedPointer.h $T/engine/CCountedPointer.h; cp $A/work/hdrA/engine/NUISystem_CAction.h $T/engine/NUISystem_CAction.h ;; *) cp $A/hdr/engine/CCountedPointer.h $T/engine/CCountedPointer.h ;; esac
  case $k in H) spec="$a=$src"; kk=H ;; N) spec="$a=$A/rows/$a.cpp"; kk=N ;; *) spec="$a@$k=$A/work/var/${a}_$k.cpp"; kk=N ;; esac
  FABLE_HDR_OVERLAY=$T timeout 900 python3 $A/tools/pass.py "$tag$k" $kk "$spec"
  md5sum $T/engine/CCountedPointer.h $T/engine/NUISystem_CAction.h | sed "s/^/$tag $k hdr /" >> $A/work/runprobe_hdr.log
  cp $A/orig/hdr_NUISystem_CAction/NUISystem_CAction.h $T/engine/NUISystem_CAction.h
done
cp $A/orig/hdr_CCountedPointer/CCountedPointer.h $T/engine/CCountedPointer.h
echo RUNPROBE_DONE $tag $a "$@" >> $A/work/runprobe_done.log
