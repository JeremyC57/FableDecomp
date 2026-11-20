#!/bin/bash
# isite.sh <src.cpp> [callee-substr] : inline-trace; print decisions for callee in the LAST function (SetAsBlocked)
src=$(readlink -f $1); pat=${2:-IsEveryUniqueIDOpen}; out=$W/tmp/c2/$(basename $src .cpp).json
cd $W/tmp/c2 && timeout 900 python3 $W/tools/c2/inl_host.py $src $out >/dev/null 2>&1 || { echo TRACEFAIL; exit 1; }
python3 - $out "$pat" <<'PY'
import json,sys
FN=__import__('os').environ.get('FN','SetAsBlocked')
ev=json.load(open(sys.argv[1])); rows=[]; last=[]
for e in ev:
    if e[0]=='pipe': last=rows; rows=[] if FN not in e[1] else rows; 
    if e[0]=='pipe' and FN in e[1]: break
    if e[0]=='d': rows.append(e[1])
for d in rows:
    if sys.argv[2] in d['n']: print("   f=%.4f c1=%s cE=%s b4e=%s sc=%s %s"%(d["flo"]/1e8+d["fhi"]*42.94967296,d.get("c1"),d.get("cE"),d.get("b4e"),d.get("sc"),"EXPAND" if d["r"] else "call"))
PY
