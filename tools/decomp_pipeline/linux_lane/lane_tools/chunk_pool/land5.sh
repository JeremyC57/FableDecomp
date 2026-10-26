#!/bin/bash
export S=${S:?set S to your scratchpad}; source $S/wine/env.sh; cd /home/user/FableDecomp
for n in "$@"; do
  A=$S/w22/agent$n; python3 tools/decomp_pipeline/linux_lane/lane_tools/naming/conv.py $A/payload.json $A/oracle.tsv $(ls $A/src/*.cpp $A/test/*.cpp $A/differ/* 2>/dev/null) >/dev/null 2>&1  # owner 2026-10-25: _STL -> std (retail)
  B=rebuild/candidates/manual/batches/2026-10-24_linux_hand$n; mkdir -p $B $A/landed_already; cp $A/payload.json $A/oracle.tsv $B/
  python3 - $A $B <<'PY'
import json,sys
A,B=sys.argv[1:]; cat={l.split("\t")[0].lower() for l in open("rebuild/oracles/auto-re-candidates.tsv")}
p=json.load(open(B+"/payload.json")); au=p["result"]["authored"]
for c in au:
    if c["address"].lower() in cat:
        a=c["address"]; open(f"{A}/landed_already/{a}.cpp","w").write(c["source_cpp"]); open(f"{A}/landed_already/{a}_test.cpp","w").write(c["test_cpp"]); print("ALREADY-LANDED (swap in):",a)
p["result"]["authored"]=[c for c in au if c["address"].lower() not in cat]; json.dump(p,open(B+"/payload.json","w"),indent=1)
rows=open(B+"/oracle.tsv").read().splitlines(); open(B+"/oracle.tsv","w").write("\n".join([rows[0]]+[r for r in rows[1:] if r.split("\t")[0].lower() not in cat])+"\n")
PY
  python3 tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/fixmove.py $B/payload.json
  python3 tools/decomp_pipeline/linux_lane/lane_tools/naming/keypayload.py $B/payload.json  # owner 2026-10-26: class key = retail (PDB)
  if [ "$(ls $A/src/*.cpp 2>/dev/null | wc -l)" -eq 0 ]; then echo "ASM in $n, skipped (no src files to purity-check)"; continue; fi
  if grep -lE '(^|[^A-Za-z0-9_])(__asm|_emit|volatile|naked)([^A-Za-z0-9_]|$)' $A/src/*.cpp $A/test/*.cpp; then echo "ASM in $n, skipped"; continue; fi
  LANDVERIFY_WORK=$S/lv$n timeout 5000 python3 -u tools/decomp_pipeline/linux_lane/lane_tools/vland.py $B/payload.json $B/oracle.tsv --land > $S/lv$n.land.log 2>&1
  echo "chunk $n: $(grep -c . $B/oracle.tsv) oracle lines; $(grep -E '^WINS' $S/lv$n.land.log)"
  grep -E "^[0-9a-f]{8} " $S/lv$n.land.log | grep -vE "WARN|PASS *[^ ]*$" | grep -vE "MATCH +PASS" | head -20
done
echo ALLDONE
