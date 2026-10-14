#!/usr/bin/env python3
"""Annotate a batch oracle: manifest prototype, callee labels (+landed flag), literals, header availability.
Usage: info.py <oracle.tsv>   (env S = scratchpad, cwd = repo)"""
import csv, struct, sys, re, os
from pathlib import Path
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86 import X86_OP_IMM, X86_OP_MEM
S=Path(os.environ["S"]); data=(S/"dl/game/Fable The Lost Chapters/Fable.exe").read_bytes()
e=struct.unpack_from("<I",data,0x3C)[0]; coff=e+4; nsec=struct.unpack_from("<H",data,coff+2)[0]; opt=struct.unpack_from("<H",data,coff+16)[0]
secbase=coff+20+opt; secs=[]
for i in range(nsec):
    o=secbase+i*40; va,=struct.unpack_from("<I",data,o+12); rp,=struct.unpack_from("<I",data,o+20); rs,=struct.unpack_from("<I",data,o+16); vs,=struct.unpack_from("<I",data,o+8); nm=data[o:o+8].rstrip(b"\0").decode(); secs.append((va,max(vs,rs),rp,nm))
def off(va):
    rva=va-0x400000
    for va0,sz,rp,nm in secs:
        if va0<=rva<va0+sz: return rp+rva-va0, nm
    return None, None
def lit(va):
    o,nm=off(va)
    if o is None: return ""
    end=data.find(b"\0",o,o+120)
    if end>o:
        s=data[o:end]
        if all(32<=c<127 for c in s): return f'"{s.decode()}"'
    w=data[o:o+160].decode("utf-16le","replace"); w=w[:w.index("\0")] if "\0" in w else ""
    if len(w)>=2 and all(32<=ord(c)<127 for c in w): return f'L"{w}"'
    if nm==".rdata":
        f,=struct.unpack_from("<f",data,o); d,=struct.unpack_from("<I",data,o)
        return f"float {f!r} / 0x{d:08x}"
    return ""
man={r["address"].lower():r for r in csv.DictReader(open("rebuild/manifest/functions.tsv",encoding="utf-8-sig"),delimiter="\t")}
cat=Path("rebuild/build_candidates.ps1").read_text(); landed=set(a.lower() for a in re.findall(r"Address\s*=\s*'([0-9a-fA-F]{8})'",cat))
hdr=set(p.stem for p in Path("rebuild/include/engine").glob("*.h"))
md=Cs(CS_ARCH_X86,CS_MODE_32); md.detail=True
for r in csv.DictReader(open(sys.argv[1],encoding="utf-8-sig"),delimiter="\t"):
    a=int(r["address"],16); b=bytes.fromhex(r["bytes"]); m=man.get(r["address"].lower(),{})
    print(f"==== {r['address']} {m.get('module')}::{m.get('name')} cc={m.get('calling_convention')} ret={m.get('return_type')} params={m.get('parameter_types')} len={r['length']} hdr={'Y' if m.get('module') in hdr else 'n'}")
    for i in md.disasm(b,a):
        note=""
        for op in i.operands:
            v=None
            if op.type==X86_OP_IMM: v=op.imm
            elif op.type==X86_OP_MEM and op.mem.base==0 and op.mem.index==0: v=op.mem.disp
            if v and 0x400000<=v<0x1500000:
                if i.mnemonic in ("call","jmp") and op.type==X86_OP_IMM:
                    t=man.get(f"{v:08x}",{}); note+=f"  -> {t.get('module')}::{t.get('name')} [{t.get('calling_convention')} ret={t.get('return_type')} p={t.get('parameter_types')}]{' LANDED' if f'{v:08x}' in landed else ''}"
                elif i.mnemonic.startswith("j"): pass
                else:
                    o,nm=off(v); note+=f"  ; {nm} {lit(v)}"
        print(f"  {i.address:08x}  {i.bytes.hex():18} {i.mnemonic} {i.op_str}{note}")
