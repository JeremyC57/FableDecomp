#!/bin/bash
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad
source $S/wine/env.sh; mkdir -p /run/user/0; export W=$S/w24/hand2c FABLE_HDR_OVERLAY=$S/w24/hand2c/hdr; cd /home/user/FableDecomp
mkdir -p $W/tmp/base
for a in $(ls $W/ref); do
  src=$W/ref/$a/draft.cpp; [ -f $src ] || src=$(ls $W/ref/$a/prior_$a*.cpp | grep -v test | head -1)
  timeout 300 python3 $W/tools/g.py $a $src "baseline draft" > $W/tmp/base/$a.txt 2>&1
  echo -e "$a\t$(head -1 $W/tmp/base/$a.txt | awk '{print $3}')\t$(grep -c '^@@' $W/tmp/base/$a.txt)\t$(grep -c '^[-+] ' $W/tmp/base/$a.txt)\t$(basename $src)" >> $W/tmp/base/summary.tsv
done
echo DONE >> $W/tmp/base/summary.tsv
