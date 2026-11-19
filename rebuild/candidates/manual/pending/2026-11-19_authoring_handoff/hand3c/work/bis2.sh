cd /home/user/FableDecomp; export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh; export W=$S/w24/hand2c FABLE_HDR_OVERLAY=$S/w24/hand2c/hdr
for k in c4 c5; do s=$(date +%s); r=$(python3 $W/tools/cc.py $W/work/bis_$k.cpp 60 | head -6 | tr '\n' ' '); echo "bis_$k $(( $(date +%s)-s ))s: $r"; done
