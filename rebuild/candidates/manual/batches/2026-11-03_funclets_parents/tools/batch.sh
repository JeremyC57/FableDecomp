#!/bin/bash
# usage: batch.sh <name> <addr>...
set -u
cd /home/user/FableDecomp
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh; export S
T=tools/decomp_pipeline/linux_lane/lane_tools
N=$1; shift; B=$S/funclets/$N; mkdir -p $B
python3 $S/funclets/mkpayload.py $B "$@"
python3 $T/chunk_pool/fixmove.py $B/payload.json; python3 $T/naming/keypayload.py $B/payload.json
LANDVERIFY_WORK=$B/work timeout 1800 python3 -u $T/vland.py $B/payload.json $B/oracle.tsv > $B/verify_readonly.log 2>&1
python3 $S/funclets/offcheck.py $B/payload.json $B/oracle.tsv $B/work > $B/offcheck.txt 2>&1
# passes: plain RELOCATION_MATCH/MATCH (no pragma/flag sweep tag) + PASS + offset at expected place
python3 - "$B" <<'PY'
import sys,json,re
B=sys.argv[1]
st={}
for l in open(f"{B}/verify_readonly.log"):
    m=re.match(r"^([0-9a-f]{8})\s+(\S+)\s+(\S+)\s+Unwind@",l)
    if m: st[m.group(1)]=(m.group(2),m.group(3))
off={l.split()[0]:l.split()[-1] for l in open(f"{B}/offcheck.txt") if l.strip()}
d=json.load(open(f"{B}/payload.json")); keep=[]; rep=[]
for c in d["result"]["authored"]:
    a=c["address"]; s,b=st.get(a,("NONE","-"))
    ok= s in ("MATCH","RELOCATION_MATCH") and b=="PASS" and off.get(a)=="OK"
    rep.append(f"{a}\t{s}\t{b}\t{off.get(a,'-')}\t{'PASS' if ok else 'PARK'}")
    if ok: keep.append(c)
json.dump({"result":{"authored":keep}},open(f"{B}/payload_land.json","w"),indent=1)
open(f"{B}/grades.tsv","w").write("\n".join(rep)+"\n"); print(len(keep),"of",len(rep),"pass")
PY
