#!/usr/bin/env bash
# runprobe.sh <tag> <addr file> [kinds, default "H R W N"] [pre]: per row at ONE path (work/one/<addr>/), one `timeout 900` pass.py grade
# per kind, every kind through ONE full include tree work/tree (a copy of rebuild/include) whose engine/CDefinitionManager.h is swapped
# before each pass, so MSVC's includer-directory lookup can never fall back to the repo header:
#   H  repo HEAD source,                     HEAD header (orig/hdr_CDefinitionManager)  -> work/obj/H_<a>.obj
#   R  work/var/<a>_R.cpp (REREAD only),     HEAD header                                -> work/obj/<a>_r.obj
#   W  work/var/<a>_W.cpp (literal wrap),    work/hdrW header                           -> work/obj/<a>_w.obj
#   N  rows/<a>.cpp (REREAD + opt-ins),      hdr/ header                                -> work/obj/<a>.obj
#   X  (dropped: wine cl hangs on a hit #error, GOTCHAS agentNF24; the overlay proof is N v R: only the overlay header reads the new macros)
# With a 5th argument `pre`, runs the clang precheck of the same (source, tree) pairs instead (work/pre.log). ONE runner at a time.
A=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF349
T=$A/work/tree
K=${3:-H R W N}
sw() { case $1 in H|R) cp $A/orig/hdr_CDefinitionManager/CDefinitionManager.h $T/engine/CDefinitionManager.h ;;
                  W) cp $A/work/hdrW/engine/CDefinitionManager.h $T/engine/CDefinitionManager.h ;;
                  N) cp $A/hdr/engine/CDefinitionManager.h $T/engine/CDefinitionManager.h ;;
                  X) cp $A/work/hdrX/engine/CDefinitionManager.h $T/engine/CDefinitionManager.h ;; esac; }
while read -r a; do
  [ -n "$a" ] || continue
  src=$(python3 -c "import sys; sys.path.insert(0,'$A/tools'); import nf; print(nf.src_of('$a'))")
  for k in $K; do
    sw $k
    case $k in H) f=$src; spec="$a=$src" ;; R) f=$A/work/var/${a}_R.cpp; spec="$a@R=$f" ;; W) f=$A/work/var/${a}_W.cpp; spec="$a@W=$f" ;;
               N) f=$A/rows/$a.cpp; spec="$a=$f" ;; X) f=$A/rows/$a.cpp; spec="$a@X=$f" ;; esac
    if [ "$4" = pre ]; then NF_TREE=$T python3 $A/tools/nf.py pre ${k}_$a "$f"; continue; fi
    if [ $k = H ]; then kk=H; else kk=N; fi
    FABLE_HDR_OVERLAY=$T timeout 900 python3 $A/tools/pass.py "${1}$k" $kk "$spec"
  done
done < "$2"
sw H
echo RUNPROBE_DONE $1 >> $A/work/runprobe_done.log
