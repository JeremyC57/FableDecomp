# bcheck.py <addr> <leaf>: reloc-masked byte differences of the last q.py build of <addr>
import sys,os,csv
sys.path.insert(0,"/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
from pathlib import Path
S=os.environ["S"]; a=sys.argv[1]; leaf=sys.argv[2]; obj=Path(f"{S}/wine/q859/{a}_{sys.argv[3]}/{a}.obj")
res=V.obj_text(obj,leaf); built,sec=res[0],res[1]; roff=res[3] if len(res)>3 else 0
rel=[r-roff for r in V.obj_relocs(obj,sec) if r>=roff]
for r in csv.DictReader(open(S+"/batch/chunk859_oracle.tsv",encoding="utf-8-sig"),delimiter="\t"):
    if r["address"]==a: rb=bytes.fromhex(r["bytes"])
while rb and rb[-1] in (0xcc,0x90): rb=rb[:-1]
ma,mb=V.mask(rb,rel),V.mask(built,rel)
d=[i for i in range(min(len(ma),len(mb))) if ma[i]!=mb[i]]
print(len(rb),len(built),len(d),[hex(int(a,16)+i) for i in d[:20]])
