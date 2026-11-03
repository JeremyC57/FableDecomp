#!/bin/bash
# mkseq.sh addr src inlinepat callsym T... : after each marker call _Mk?, whether the next dtor is I (inline pattern) or C (call)
I=${S}/pragma3
a=$1; f=$2; ip=$3; cs=$4; shift 4
for T in "$@"; do
  if [ "$T" = "-" ]; then fl=""; else fl="--flags /d2inlT$T"; fi
  $I/qq.sh $a $f $fl >/dev/null
  seq=$(objdump -d -r -M intel --no-show-raw-insn $I/work/$a/$(basename $f .cpp)/$a.obj | awk -v ip="$ip" -v cs="$cs" '
    /^[0-9a-f]+ <.*>:/{p=($0 !~ /<\$/)} !p{next}
    /DISP32/ && /_Mk[A-Z]/ {m=substr($NF,4,1); next}
    m!="" && index($0,cs)>0 && /DISP32/{printf "%s:C ",m; m=""; next}
    m!="" && index($0,ip)>0 {printf "%s:I ",m; m=""}')
  echo "T=$T $seq"
done
