#!/bin/bash
# usage: land_pending.sh <name>          (FINAL step -- the Fable 5.1 session runs this)
# Re-grade, behaviour-test and land a staged batch from rebuild/candidates/manual/pending/<name>/:
# copies the payload into rebuild/candidates/manual/batches/<name>/ (dropping rows already in the catalog and
# trailing int3/nop padding from the oracle rows), runs verify_and_land.py --land (it re-grades every source
# under the retail flags + pragma sweep, runs every test, rejects asm), writes the batch README from NOTES.md,
# then trims the pending dir to what is still open (differ/, rows.tsv, NOTES.md, STATUS.tsv).
set -e
: "${S:?export S=<scratch lane root> and source \$S/wine/env.sh first}"
cd "$(git rev-parse --show-toplevel)"
P=rebuild/candidates/manual/pending/$1; D=rebuild/candidates/manual/batches/$1
[ -f "$P/payload.json" ] || { echo "nothing to land in $P"; exit 1; }
mkdir -p "$D"
python3 - "$P" "$D" <<'PY'
import json,sys,re
P,D=sys.argv[1],sys.argv[2]
auth=json.load(open(P+"/payload.json"))["result"]["authored"]
cat={l.split("\t")[0].lower() for l in open("rebuild/oracles/auto-re-candidates.tsv")}
keep=[c for c in auth if c["address"].lower() not in cat]
for c in keep:
    c.setdefault("pass_pattern", f"{c['address']}_TEST PASS")
    if c.get("flags") in ([],None): c.pop("flags",None)
json.dump({"result":{"authored":keep}},open(D+"/payload.json","w"),indent=1)
rows=open(P+"/oracle.tsv").read().splitlines(); body=[]
ka={c["address"].lower() for c in keep}
for r in rows[1:]:
    a,n,l,h=r.split("\t")
    if a.lower() not in ka: continue
    b=bytes.fromhex(h)
    while b and b[-1] in (0xcc,0x90): b=b[:-1]
    body.append(f"{a}\t{n}\t{len(b)}\t{b.hex()}")
open(D+"/oracle.tsv","w").write("\n".join([rows[0]]+body)+"\n")
for c in keep:
    base=re.sub(r"[^A-Za-z0-9]","",c["module"])+"_"+re.sub(r"[^A-Za-z0-9]","",c["name"])+"_"+c["address"]
    open(f"{D}/{base}.cpp","w").write(c["source_cpp"]); open(f"{D}/{base}_test.cpp","w").write(c["test_cpp"])
print(len(auth),"staged,",len(keep),"to land")
PY
python3 -u tools/decomp_pipeline/verify_and_land.py "$D/payload.json" "$D/oracle.tsv" --land 2>&1 | grep -E "DIFFER|FAIL|WINS|LANDED|TCC|SRC_FAIL" || true
cp "$P/NOTES.md" "$D/README.md"
rm -rf "$P/payload.json" "$P/oracle.tsv" "$P/src" "$P/test"
echo "landed $1 -- compare WINS with the staged count; any row the lander refused goes to differ/ + the queue doc"
