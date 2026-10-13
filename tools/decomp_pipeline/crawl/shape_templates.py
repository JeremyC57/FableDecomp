#!/usr/bin/env python3
"""Opcode-skeleton template finder.

Indexes every landed MATCH/RELOCATION_MATCH body (<=160 bytes) by its instruction shape
(mnemonics + operand kinds, registers kept, immediates/displacements masked) and, for each
target in a JSON list [{address, module, name, len, bytes}], prints the landed sources with an
identical shape -- their C++ is the proven codegen template for the new body.

Usage: python shape_templates.py <targets.json> <out.md>
Env:   FABLE_ROOT (repo root), FABLE_EXE (retail Fable.exe); defaults are the Windows paths."""
import csv, struct, os, sys, json, re, collections
from pathlib import Path
import capstone
from capstone import x86
ROOT=Path(os.environ.get("FABLE_ROOT", r"D:\Documents\FableTLC")); EXE=Path(os.environ.get("FABLE_EXE", r"C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\Fable.exe"))
sys.path.insert(0,str(ROOT/"tools/decomp_pipeline/crawl"))
from rowtrim import trim_body
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
def body(va):
    o=off(va); nx=nextof.get(va)
    if o is None or nx is None: return b""
    raw=data[o:o+(nx-va)]; e=len(raw)
    while e>0 and raw[e-1] in (0xCC,0x90): e-=1
    return trim_body(raw[:e], va)[0]
md=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32); md.detail=True
def skeleton(b,va,keep_regs=True):
    out=[]
    for i in md.disasm(b,va):
        ops=[]
        for o in i.operands:
            if o.type==x86.X86_OP_REG: ops.append(i.reg_name(o.reg) if keep_regs else "r")
            elif o.type==x86.X86_OP_IMM: ops.append("imm")
            else:
                m=o.mem; ops.append("[%s+%s*%d+d]"%(i.reg_name(m.base) if m.base else "", i.reg_name(m.index) if m.index else "", m.scale))
        out.append(i.mnemonic+" "+",".join(ops))
    return "|".join(out)
# landed byte-exact functions
par=list(csv.DictReader(open(ROOT/"rebuild/compile-gate/retail-parity.tsv",encoding="utf-8-sig"),delimiter="\t"))
comp={r["address"].strip('"').lower():r for r in csv.DictReader(open(ROOT/"rebuild/compile-gate/vc71-compiled.tsv",encoding="utf-8-sig"),delimiter="\t")}
index=collections.defaultdict(list)
for r in par:
    if not r["status"].startswith(("MATCH","RELOCATION")): continue
    a=r["address"].lower(); ai=int(a,16)
    src=comp.get(a,{}).get("source","")
    rel=src.replace("D:\\Documents\\FableTLC\\","").replace("\\","/")
    if not (ROOT/rel).exists(): continue
    b=body(ai)
    if not b or len(b)>160: continue
    index[skeleton(b,ai)].append((a,rel,len(b)))
print("indexed skeletons",len(index),file=sys.stderr)
targets=json.load(open(sys.argv[1]))
with open(sys.argv[2],"w") as f:
    for t in targets:
        b=bytes.fromhex(t["bytes"]); va=int(t["address"],16)
        sk=skeleton(b,va); hits=index.get(sk,[])
        f.write(f"## {t['address']} {t['module']}::{t['name']} len={t['len']} hits={len(hits)}\n")
        for h in hits[:3]: f.write(f"   template {h[0]} {h[1]}\n")
        if not hits:
            # fuzzy: same mnemonics sequence ignoring regs
            sk2=skeleton(b,va,False)
            f.write("   (no exact-shape template)\n")
