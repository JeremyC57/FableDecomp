# relpair.py <addr> <leaf>: every relocation of the last q.py build with its symbol and the retail target at that offset
import sys,os,csv,re,subprocess,struct
sys.path.insert(0,"/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
from pathlib import Path
S=os.environ["S"]; a=sys.argv[1]; leaf=sys.argv[2]; obj=Path(f"{S}/wine/q/{a}/{a}.obj")
rb=None
for r in csv.DictReader(open(S+"/batch/chunk855_oracle.tsv",encoding="utf-8-sig"),delimiter="\t"):
    if r["address"].lower()==a: rb=bytes.fromhex(r["bytes"])
res=V.obj_text(obj,leaf); built,sec=res[0],res[1]; roff=res[3] if len(res)>3 else 0
# section index -> reloc records with symbols
rp=subprocess.run([V.OBJDUMP,"-r",str(obj)],capture_output=True,text=True).stdout
hp=subprocess.run([V.OBJDUMP,"-h",str(obj)],capture_output=True,text=True).stdout
blocks=[];cur=None
for l in rp.splitlines():
    if l.startswith("RELOCATION RECORDS FOR ["):
        cur=[] ; blocks.append((l,cur)); continue
    m=re.match(r"^([0-9a-f]{8}) (\S+)\s+(.*)$",l)
    if m and cur is not None: cur.append((int(m.group(1),16),m.group(2),m.group(3)))
textblocks=[b for b in blocks if V.TEXTSEC_R.match(b[0])]
# pick the block whose offsets match the masked relocs
rel=set(r for r in V.obj_relocs(obj,sec))
best=None
for hdr,recs in textblocks:
    if rel and set(o for o,_,_ in recs)==rel: best=recs
if best is None: best=max(textblocks,key=lambda b:len(set(o for o,_,_ in b[1])&rel))[1]
base=int(a,16)
for o,t,sym in best:
    o2=o-roff
    if o2<0: continue
    v=struct.unpack_from("<I",rb,o2)[0] if o2+4<=len(rb) else 0
    if t.endswith("REL32") or "PCRELLONG" in t or t=="DISP32": v=(base+o2+4+v)&0xffffffff
    print(f"{base+o2:08x} {t:10} {v:08x} {sym}")
