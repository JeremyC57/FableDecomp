#!/bin/bash
# at.sh <V> [users...]: grade AllocateType variant V on row 00a4efc0 (e1 base) and on landed users (config a)
S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad; W=$S/w22/auth2; D=$W/00a4efc0
export FABLE_HDR_OVERLAY=$D/hdr
v=$1; shift
mkdir -p $D/at
sed "s/^#define FABLE_ALLOCATETYPE_ASSERT_COND false/&\n#define FABLE_AT_V $v/" $S/w22/auth1/00a4efc0/e1.cpp > $D/at/row_v$v.cpp
bash $S/w22/auth1/g.sh 00a4efc0 '?InitialiseFromFileData@C3DAnimationSequenceData@@QAE_NABVC3DAnimFileSequenceChunk@@AAVCSimpleMemoryPool@@@Z' $D/at/row_v$v.cpp -- /Gr
for a in "$@"; do
  sed "s/^#define FABLE_ALLOCATETYPE_ASSERT_COND false/&\n#define FABLE_AT_V $v/" $D/cal/${a}_a.cpp > $D/at/${a}_v$v.cpp
  leaf=$(grep -m1 "^// Leaf:" $D/cal/${a}_0.cpp | awk '{print $3}')
  bash $S/w22/auth1/g.sh $a "$leaf" $D/at/${a}_v$v.cpp
done
