#!/usr/bin/env python3
"""Portable picker: smallest un-landed manifest functions with complete prototype + known CC.
Usage: pick.py <N> <out_prefix> [min_len] [max_len]
Env: FABLE_ROOT (repo), FABLE_EXE (retail exe), OUT_DIR"""
import csv, struct, re, os, sys, json
from pathlib import Path
ROOT=Path(os.environ["FABLE_ROOT"]); EXE=Path(os.environ["FABLE_EXE"]); OUT=Path(os.environ.get("OUT_DIR","."))
sys.path.insert(0,str(ROOT/"tools/decomp_pipeline/crawl"))
from rowtrim import trim_body
N=int(sys.argv[1]); PREFIX=sys.argv[2]; MINLEN=int(sys.argv[3]) if len(sys.argv)>3 else 12; MAXLEN=int(sys.argv[4]) if len(sys.argv)>4 else 140
data=EXE.read_bytes()
e=struct.unpack_from("<I",data,0x3C)[0]; coff=e+4
nsec=struct.unpack_from("<H",data,coff+2)[0]; opt=struct.unpack_from("<H",data,coff+16)[0]
secbase=coff+20+opt; IMG=0x400000; secs=[]
for i in range(nsec):
    o=secbase+i*40
    va,=struct.unpack_from("<I",data,o+12); rp,=struct.unpack_from("<I",data,o+20)
    rs,=struct.unpack_from("<I",data,o+16); vs,=struct.unpack_from("<I",data,o+8)
    secs.append((va,max(vs,rs),rp))
def off(va):
    rva=va-IMG
    for va0,sz,rp in secs:
        if va0<=rva<va0+sz: return rp+(rva-va0)
rows=list(csv.DictReader(open(ROOT/"rebuild/manifest/functions.tsv",encoding="utf-8-sig"),delimiter="\t"))
addrs=sorted(int(r["address"],16) for r in rows if r.get("address"))
nextof={addrs[i]:addrs[i+1] for i in range(len(addrs)-1)}
cat=(ROOT/"rebuild/build_candidates.ps1").read_text(encoding="utf-8")
landed=set(int(m,16) for m in re.findall(r"Address\s*=\s*'([0-9a-fA-F]{8})'",cat))
tried=set(int(x,16) for x in (ROOT/"tools/decomp_pipeline/crawl/gen_tried.txt").read_text().split())
def body(va):
    o=off(va); nx=nextof.get(va)
    if o is None or nx is None: return b""
    raw=data[o:o+(nx-va)]; e=len(raw)
    while e>0 and raw[e-1] in (0xCC,0x90): e-=1
    return trim_body(raw[:e], va)[0]
cand=[]
for r in rows:
    a=r.get("address")
    if not a: continue
    ai=int(a,16)
    if ai in landed or ai in tried or r.get("retail_parity") or r.get("compiled_status"): continue
    if r.get("prototype_complete")!="1": continue
    if r.get("calling_convention","") in ("","unknown"): continue
    b=body(ai)
    if len(b)<MINLEN or len(b)>MAXLEN or b.startswith(b"\xff\x25"): continue
    cand.append((len(b),ai,r,b))
cand.sort(key=lambda x:(x[0],x[1]))
print("eligible",len(cand))
sel=cand[:N]
with open(OUT/(PREFIX+"_oracle.tsv"),"w",newline="") as f:
    w=csv.writer(f,delimiter="\t",lineterminator="\n"); w.writerow(["address","name","length","bytes"])
    for ln,ai,r,b in sel: w.writerow(["%08x"%ai,r["name"],ln,b.hex()])
with open(OUT/(PREFIX+"_targets.json"),"w") as f:
    json.dump([{"address":"%08x"%ai,"name":r["name"],"module":r["module"],"cc":r["calling_convention"],
                "ret":r["return_type"],"params":r["parameter_types"],"len":ln,"bytes":b.hex()} for ln,ai,r,b in sel],f,indent=1)
for ln,ai,r,b in sel: print("%08x %3d %-10s %-6s %-40s %s(%s)"%(ai,ln,r["calling_convention"],r["return_type"],(r["module"]+"::"+r["name"])[:60],"",r["parameter_types"]))
