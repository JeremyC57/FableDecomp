#!/bin/bash
# tseq.sh addr src inlinepattern callsym T... : per threshold, the layout-order sequence of I (inline expansion marker) / C (call)
I=${S}/pragma3
a=$1; f=$2; ip=$3; cs=$4; shift 4
for T in "$@"; do
  if [ "$T" = "-" ]; then fl=""; else fl="--flags /d2inlT$T"; fi
  r=$($I/qq.sh $a $f $fl | head -1 | awk '{print $2}')
  seq=$(objdump -d -r -M intel --no-show-raw-insn $I/work/$a/$(basename $f .cpp)/$a.obj | awk -v ip="$ip" -v cs="$cs" '
    /^[0-9a-f]+ <.*>:/{p=($0 !~ /<\$/)} !p{next}
    index($0,cs)>0 && /DISP32/{printf "C"; next}
    index($0,ip)>0 {printf "I"}')
  echo "T=$T $r $seq"
done
