import json,re,bisect,sys,os
S=os.environ["S"]
ehm=json.load(open(f"{S}/funclets/ehmap.json"))
ps=open("rebuild/build_candidates.ps1",encoding="utf-8-sig").read()
ent={}
for blk in re.findall(r"\[pscustomobject\]@\{(.*?)\n    \}", ps, re.S):
    kv=dict(re.findall(r"(\w+) = '([^']*)'", blk))
    if "Address" in kv: ent[kv["Address"].lower()]=kv
cat={}
for l in open("rebuild/oracles/auto-re-candidates.tsv").read().splitlines()[1:]:
    q=l.split("\t"); cat[q[0].lower()]=q
man=[l.split("\t") for l in open("rebuild/manifest/functions.tsv").read().splitlines()[1:]]
mname={q[0].lower():q[1] for q in man}
isf=lambda n: ("Unwind@" in n) or ("Catch@" in n)
starts=sorted(int(q[0],16) for q in man if not isf(q[1]))
cov={}
for l in open("ghidra_out/coverage.tsv",encoding="utf-8-sig"):
    f=l.rstrip("\n").split("\t")
    if len(f)>3 and re.match(r"^[0-9a-f]{8}$",f[0]): cov[f[0]]=int(f[3])
ref={}
for fi,r in ehm.items():
    for k,to,act in r["acts"]:
        if act: ref.setdefault(act,[]).append((fi,"unwind",k))
    for t,c,h in r["catches"]:
        ref.setdefault(h,[]).append((fi,"catch",t,c))
mansorted=sorted(man,key=lambda q:int(q[0],16))
alls=[int(q[0],16) for q in mansorted]
rows=[]
for i,q in enumerate(mansorted):
    a=q[0].lower(); nm=q[1]
    if not isf(nm) or a in cat: continue
    A=int(a,16); refs=ref.get(A,[])
    nxt=alls[i+1] if i+1<len(alls) else 0
    fis=sorted(set(x[0] for x in refs))
    parents=[]
    for fi in fis:
        for p in ehm[fi]["push"]:
            j=bisect.bisect_right(starts,p)-1; parents.append("%08x"%starts[j])
    parents=sorted(set(parents))
    par=parents[0] if len(parents)==1 else ""
    kind=";".join(sorted(set(x[1] for x in refs))) or "none"
    rows.append([a,nm,kind,",".join(fis),",".join(parents),mname.get(par,"") if par else "",
                 "landed" if par in ent else ("cat" if par in cat else "unlanded"),ent.get(par,{}).get("Source",""),
                 ent.get(par,{}).get("CompilerFlags",""),str(cov.get(a,"")),str(nxt-A)])
hdr="address\tname\tref_kind\tfuncinfo\tparent\tparent_name\tparent_state\tparent_source\tparent_flags\tcov_len\tmanifest_gap"
open(sys.argv[1],"w").write(hdr+"\n"+"\n".join("\t".join(r) for r in rows)+"\n")
from collections import Counter
print(Counter((r[2],r[6],len(r[4].split(",")) if r[4] else 0) for r in rows))
