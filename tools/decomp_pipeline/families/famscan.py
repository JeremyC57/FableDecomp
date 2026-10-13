import csv, struct, sys, re, json, os, collections
from pathlib import Path
sys.path.insert(0, "tools/decomp_pipeline/crawl")
from rowtrim import trim_body
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86 import X86_OP_IMM, X86_OP_MEM, X86_OP_REG
S=Path(os.environ["S"]); data=(S/"dl/game/Fable The Lost Chapters/Fable.exe").read_bytes()
e=struct.unpack_from("<I",data,0x3C)[0]; coff=e+4; nsec=struct.unpack_from("<H",data,coff+2)[0]; opt=struct.unpack_from("<H",data,coff+16)[0]
secbase=coff+20+opt; secs=[]
for i in range(nsec):
    o=secbase+i*40; va,=struct.unpack_from("<I",data,o+12); rp,=struct.unpack_from("<I",data,o+20); rs,=struct.unpack_from("<I",data,o+16); vs,=struct.unpack_from("<I",data,o+8); secs.append((va,max(vs,rs),rp))
def off(va):
    rva=va-0x400000
    for va0,sz,rp in secs:
        if va0<=rva<va0+sz: return rp+rva-va0
MAXB=int(sys.argv[1]) if len(sys.argv)>1 else 64
rows=list(csv.DictReader(open("rebuild/manifest/functions.tsv",encoding="utf-8-sig"),delimiter="\t"))
cat=Path("rebuild/build_candidates.ps1").read_text(); landed=set(a.lower() for a in re.findall(r"Address\s*=\s*'([0-9a-fA-F]{8})'",cat))
tried=set(l.strip() for l in open("tools/decomp_pipeline/crawl/gen_tried.txt"))
addrs=sorted(int(r["address"],16) for r in rows if r["address"])
nxt={a:b for a,b in zip(addrs, addrs[1:])}
md=Cs(CS_ARCH_X86,CS_MODE_32); md.detail=True
fams=collections.defaultdict(list); n=0; dbg=0
for r in rows:
    a=r["address"].lower()
    if not a or a in landed or a in tried: continue
    va=int(a,16); o=off(va)
    if o is None: continue
    end=nxt.get(va, va+MAXB); size=min(end-va, MAXB+32)
    if size<5: continue
    b=data[o:o+size]
    try: b=trim_body(b, va)[0]
    except Exception as ex: pass
    if not b or len(b)<5 or len(b)>MAXB: continue
    sk=[]; total=0; ends=False
    for i in md.disasm(b, va):
        ops=[]
        for op in i.operands:
            if op.type==X86_OP_REG: ops.append("r")
            elif op.type==X86_OP_IMM: ops.append("t" if (i.mnemonic in ("call","jmp") or i.mnemonic.startswith("j")) else "i")
            elif op.type==X86_OP_MEM: ops.append("m"+("b" if op.mem.base else "")+("x" if op.mem.index else "")+("d" if op.mem.disp else ""))
        sk.append(i.mnemonic+"("+",".join(ops)+")"); total+=i.size
        if i.mnemonic=="ret" or (i.mnemonic=="jmp" and total==len(b)): ends=True
    if not ends or total!=len(b): continue
    key=" ".join(sk); fams[key].append((a, r["module"], r["name"], len(b), b.hex())); n+=1
print("bodies scanned:", n, "families:", len(fams), "singletons:", sum(1 for v in fams.values() if len(v)==1))
big=sorted(fams.items(), key=lambda kv:-len(kv[1]))
cum=0
for k,v in big[:60]:
    cum+=len(v)
    print(f"{len(v):5d} (cum {cum:5d}) {v[0][3]:3d}B  {k[:140]}")
    print(f"        e.g. {v[0][0]} {v[0][1]}::{v[0][2]} | {v[1][0]} {v[1][2]} | {v[2][0] if len(v)>2 else ''}")
json.dump([(k, v) for k,v in big], open(os.environ.get("FAMOUT", str(S/"batch/families.json")),"w"))
