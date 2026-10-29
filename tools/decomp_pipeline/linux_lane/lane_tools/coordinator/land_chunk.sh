: "${S:?export S=<lane scratch dir> first}"; source $S/wine/env.sh; cd "$(git rev-parse --show-toplevel)"
n=${1:?usage: land_chunk.sh N};  A=$S/w22/agent$n; T=tools/decomp_pipeline/linux_lane/lane_tools
python3 $T/naming/conv.py $A/payload.json $A/oracle.tsv $(ls $A/src/*.cpp $A/test/*.cpp $A/differ/* 2>/dev/null) >/dev/null 2>&1
B=rebuild/candidates/manual/batches/2026-10-27_linux_hand$n; mkdir -p $B $A/landed_already; cp $A/payload.json $A/oracle.tsv $B/
python3 - $A $B <<'PY'
import json,sys
A,B=sys.argv[1:]; cat={l.split("\t")[0].lower() for l in open("rebuild/oracles/auto-re-candidates.tsv")}
p=json.load(open(B+"/payload.json")); au=p["result"]["authored"]
for c in au:
    if c["address"].lower() in cat: print("ALREADY-LANDED:",c["address"])
p["result"]["authored"]=[c for c in au if c["address"].lower() not in cat]; json.dump(p,open(B+"/payload.json","w"),indent=1)
rows=open(B+"/oracle.tsv").read().splitlines(); open(B+"/oracle.tsv","w").write("\n".join([rows[0]]+[r for r in rows[1:] if r.split("\t")[0].lower() not in cat])+"\n")
print("payload",len(p["result"]["authored"]))
PY
python3 $T/chunk_pool/fixmove.py $B/payload.json
python3 $T/naming/keypayload.py $B/payload.json
if grep -lE '(^|[^A-Za-z0-9_])(__asm|_emit|volatile|naked)([^A-Za-z0-9_]|$)' $A/src/*.cpp $A/test/*.cpp; then echo "ASM in src, STOP"; exit 1; fi
LANDVERIFY_WORK=$S/lv$n timeout 3000 python3 -u $T/vland.py $B/payload.json $B/oracle.tsv --land > $S/lv$n.land.log 2>&1
echo "main: $(grep -E '^WINS' $S/lv$n.land.log)"
echo ALLDONE
