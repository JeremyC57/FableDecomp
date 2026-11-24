#!/bin/bash
# gr.sh <tag> <H|N> <addr>...: grade each row (H = repo HEAD source, no overlay; N = rows/<addr>.cpp with overlay hdr/) with qq.py,
# keep the object as work/obj/<H|N>_<addr>.obj and its DEF/UND symbols; log work/gr_<tag>.log. Each grade has its own timeout 900 line.
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad; source $S/wine/env.sh >/dev/null 2>&1; export S
A=$S/w22/agentNF296; tag=$1; side=$2; shift 2; mkdir -p $A/work/obj; cd $A/work
for a in "$@"; do
  if [ $side = H ]; then src=$(bash $A/tools/srcof.sh $a); ov=; P=$A/pH; else src=$A/rows/$a.cpp; ov=$A/hdr; P=$A/pS; fi
  leaf=$(grep -m1 -oP '^// Leaf:\s*\K\S+' $src); fl=$(bash $A/tools/flags.sh $a)
  set -f; fa=(); [ -n "$fl" ] && fa=(--flags $fl); set +f
  S=$P FABLE_HDR_OVERLAY=$ov timeout 900 python3 $A/tools/qq.py $a $src $leaf "${fa[@]}" > $A/work/q_${side}_$a.txt 2>&1
  echo "$side $(grep -m1 -E "^$a " $A/work/q_${side}_$a.txt | cut -c1-200) leaf=$leaf" >> $A/work/gr_$tag.log
  cp $P/wine/q/$a/$a.obj $A/work/obj/${side}_$a.obj 2>/dev/null || echo "$side $a NO_OBJ" >> $A/work/gr_$tag.log
  python3 /home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/naming_batch/clean.py $A $P/wine/q/$a > /dev/null
done
echo GR_DONE >> $A/work/gr_$tag.log
