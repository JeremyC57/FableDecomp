#!/bin/bash
# usage: land_agent.sh <agentNN> <batchname>   -- copies the agent payload into a batch dir and lands it (sequential!)
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad
source $S/wine/env.sh; export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; cd /home/user/FableDecomp
A=$S/w22/$1; D=rebuild/candidates/manual/batches/$2; mkdir -p $D
python3 - "$A" "$D" <<'PY'
import json,sys,os,re
A,D=sys.argv[1],sys.argv[2]
p=json.load(open(A+"/payload.json"))
auth=p["result"]["authored"] if "result" in p else p
cat={l.split("\t")[0].lower() for l in open("rebuild/oracles/auto-re-candidates.tsv")}
keep=[c for c in auth if c["address"].lower() not in cat]
for c in keep:
    c.setdefault("pass_pattern", f"{c['address']}_TEST PASS")
    if c.get("flags") in ([],None): c.pop("flags",None)
json.dump({"result":{"authored":keep}},open(D+"/payload.json","w"),indent=1)
rows=open(A+"/oracle.tsv").read().splitlines()
hdr=rows[0]; body=[]
for r in rows[1:]:
    a,n,l,h=r.split("\t")
    if a.lower() not in {c["address"].lower() for c in keep}: continue
    b=bytes.fromhex(h)
    while b and b[-1] in (0xcc,0x90): b=b[:-1]   # family-scan rows may carry the int3 padding; the lander compares the full row
    body.append(f"{a}\t{n}\t{len(b)}\t{b.hex()}")
open(D+"/oracle.tsv","w").write("\n".join([hdr]+body)+"\n")
for c in keep:
    base=re.sub(r"[^A-Za-z0-9]","",c["module"])+"_"+re.sub(r"[^A-Za-z0-9]","",c["name"])+"_"+c["address"]
    open(f"{D}/{base}.cpp","w").write(c["source_cpp"]); open(f"{D}/{base}_test.cpp","w").write(c["test_cpp"])
print(len(auth),"authored,",len(keep),"to land")
PY
python3 -u tools/decomp_pipeline/verify_and_land.py $D/payload.json $D/oracle.tsv --land 2>&1 | grep -E "DIFFER|FAIL|WINS|LANDED|MATCH"
echo "done $2"
