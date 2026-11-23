#!/bin/bash
# grc.sh <tag> <src-addr>:<owner-addr>...: grade rows/<src-addr>.cpp against the OWNER's retail address with the owner row's own
# `// Leaf:` symbol (proves a pick-any copy the binder defines equals the owner: 2026-11-20 pick-any rule); log work/grc_<tag>.log.
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad; source $S/wine/env.sh >/dev/null 2>&1; export S
A=$S/w22/agentNF306; tag=$1; shift; cd $A/work
for p in "$@"; do a=${p%%:*}; o=${p##*:}; src=$A/rows/$a.cpp; osrc=$(bash $A/tools/srcof.sh $o)
  leaf=$(grep -m1 -oP '^// Leaf:\s*\K\S+' $osrc); fl=$(bash $A/tools/flags.sh $a); set -f; fa=(); [ -n "$fl" ] && fa=(--flags $fl); set +f
  S=$A/pS FABLE_HDR_OVERLAY=$A/hdr timeout 900 python3 $A/tools/qq.py $o $src $leaf "${fa[@]}" > $A/work/qc_${a}_$o.txt 2>&1
  echo "COPY $a->$o $(grep -m1 -E "^$o " $A/work/qc_${a}_$o.txt | cut -c1-200) leaf=${leaf:0:60}" >> $A/work/grc_$tag.log
  python3 /home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/naming_batch/clean.py $A $A/pS/wine/q/$o > /dev/null
done; echo GRC_DONE >> $A/work/grc_$tag.log
