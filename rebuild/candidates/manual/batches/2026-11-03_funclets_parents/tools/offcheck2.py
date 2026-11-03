# usage: offcheck2.py <payload> <oracle> <workroot>
# For each funclet: which $L blocks of the object reproduce the oracle at the EXPECTED offset (relocation-masked),
# restricted to blocks whose length equals the retail block (first funclet .. __ehhandler thunk + 10). The block's
# __ehhandler$ symbol (by COFF section number from objdump -t) names the function that owns it.
# Prints: addr verdict block ehhandler blocklen leafcheck
import sys,json,os,re,subprocess
from pathlib import Path
sys.path.insert(0,"tools/decomp_pipeline"); import verify_and_land as V
S=os.environ["S"]
rows={l.split("\t")[0]:l.split("\t") for l in open(f"{S}/funclets/ROWS3.tsv").read().splitlines()[1:]}
ps=open("rebuild/build_candidates.ps1",encoding="utf-8-sig").read()
ent={}
for blk in re.findall(r"\[pscustomobject\]@\{(.*?)\n    \}", ps, re.S):
    kv=dict(re.findall(r"(\w+) = '([^']*)'", blk))
    if "Address" in kv: ent[kv["Address"].lower()]=kv
p=json.load(open(sys.argv[1]))["result"]["authored"]
orc={l.split("\t")[0]:l.split("\t") for l in open(sys.argv[2]).read().splitlines()[1:]}
for c in p:
    a=c["address"]; obj=V.shard_directory(Path(sys.argv[3]),a,leaf=True)/f"{a}.obj"
    if not obj.exists(): print(a,"NOOBJ"); continue
    r=rows[a]; rlen=int(r[15],16)-int(r[11],16)+10
    psrc=open("rebuild/src/compiled/"+ent[r[4]]["Source"],encoding="utf-8").read()
    m=re.search(r"^//\s*Leaf:\s*(\S+)",psrc,re.M); leaf=m.group(1) if m else None
    ret=bytes.fromhex(orc[a][3]); o=c["funclet_offset"]; L=len(ret)
    hd=subprocess.run(["objdump","-h",str(obj)],capture_output=True,text=True).stdout
    textidx=[int(x.group(1))+1 for x in re.finditer(r"^\s*(\d+)\s+(\.text\S*)\s",hd,re.M)]
    ehsym={}
    for l in subprocess.run(["objdump","-t",str(obj)],capture_output=True,text=True).stdout.splitlines():
        mm=re.search(r"\(sec\s+(\d+)\).*\s(__ehhandler\$\S+)$",l)
        if mm: ehsym[int(mm.group(1))]=mm.group(2)
    pr=subprocess.run(["objdump","-d","-z",str(obj)],capture_output=True,text=True).stdout
    fns,sec,cur=[],-1,None
    for line in pr.splitlines():
        if V.TEXTSEC_D.match(line): sec+=1;cur=None;continue
        s=V.DS.match(line)
        if s:
            if int(s.group(1),16)==0 or cur is None: cur={"symbol":s.group(2),"section":sec,"bytes":bytearray()};fns.append(cur)
            continue
        e=V.DB.match(line)
        if e and cur is not None: cur["bytes"].extend(bytes.fromhex(e.group(1)))
    hits=[]
    for f in fns:
        if not f["symbol"].startswith("$L") or len(f["bytes"])<o+L: continue
        rel=V.obj_relocs(obj,f["section"]); rr=[x-o for x in rel if o<=x<o+L]
        if V.mask(ret,rr)==V.mask(bytes(f["bytes"][o:o+L]),rr):
            eh=ehsym.get(textidx[f["section"]] if f["section"]<len(textidx) else -1,"?")
            hits.append((f["symbol"],eh,len(f["bytes"])))
    good=[h for h in hits if h[2]==rlen]
    if len(good)>1 and leaf: good=[h for h in good if leaf in h[1]] or good
    if len(good)==1:
        h=good[0]; lc="leaf-ok" if (leaf and leaf in h[1]) else ("leaf-none" if not leaf else "leaf-MISMATCH")
        print(a,"OK",h[0],h[1],h[2],lc)
    else:
        print(a,"AMBIG" if good else "NOHIT","-","-",rlen,"hits="+repr(hits))
