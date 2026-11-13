#!/bin/bash
# Coordinator re-verification of an agent's matched rows with a MINIMAL overlay (only the changed headers the rows include).
# usage: coordverify.sh <agent-dir> <stage-dir> <addr8>...     (needs $S/wine/env.sh sourced; run from the repo root; read-only)
A=$1; ST=$2; shift 2
rm -rf "$ST"; mkdir -p "$ST/hdr" "$ST/hdrdiff"; : > "$ST/deps.txt"
for a in "$@"; do
  g++ -M -D_MSC_VER=1310 -I "$A/hdr" -I rebuild/include "$A/src/$a.cpp" 2>/dev/null | tr ' \\' '\n\n' | grep "^$A/hdr/" >> "$ST/deps.txt"
done
sort -u -o "$ST/deps.txt" "$ST/deps.txt"
while read f; do
  r=${f#$A/hdr/}
  if [ -f "rebuild/include/$r" ]; then
    cmp -s "rebuild/include/$r" "$f" && continue
    mkdir -p "$ST/hdr/$(dirname $r)"; cp "$f" "$ST/hdr/$r"
    diff -u "rebuild/include/$r" "$f" | sed "1s|.*|--- a/rebuild/include/$r|;2s|.*|+++ b/rebuild/include/$r|" > "$ST/hdrdiff/$(basename $r).diff"
  else
    mkdir -p "$ST/hdr/$(dirname $r)"; cp "$f" "$ST/hdr/$r"; cp "$f" "$ST/hdrdiff/NEW_$(basename $r)"
  fi
done < "$ST/deps.txt"
python3 - "$A" "$ST" "$@" <<'P'
import json,sys
A,ST,addrs=sys.argv[1],sys.argv[2],[x.lower() for x in sys.argv[3:]]
a=[r for r in json.load(open(A+"/payload.json"))["result"]["authored"] if r["address"].lower().replace("0x","") in addrs]
for r in a: print(r["address"],r["module"],r["name"],r.get("flags"))
json.dump({"result":{"authored":a}},open(ST+"/payload.json","w"),indent=1)
P
head -1 "$A/oracle.tsv" | grep -i "^address" > "$ST/oracle.tsv"
for a in "$@"; do grep -i "^$a" "$A/oracle.tsv" >> "$ST/oracle.tsv"; done
LANDVERIFY_STRICT_HEADERS=1 FABLE_HDR_OVERLAY=$ST/hdr LANDVERIFY_WORK=$ST/lv timeout 1800 python3 -u tools/decomp_pipeline/linux_lane/lane_tools/vland.py "$ST/payload.json" "$ST/oracle.tsv" > "$ST/vland.log" 2>&1
rm -rf "$ST/lv"
ls "$ST/hdrdiff"; grep -v "^$" "$ST/vland.log" | grep -v "WARN RELOC_NAME" | tail -8 | cut -c1-200
