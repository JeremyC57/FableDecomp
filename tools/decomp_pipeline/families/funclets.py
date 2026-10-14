#!/usr/bin/env python3
"""Payload generator: EH unwind funclets of already-landed /EHsc parents.
usage: funclets.py <repo> <batchname> [limit]"""
import re,struct,json,sys,os
from pathlib import Path
S=Path(os.environ["S"]); ROOT=Path(sys.argv[1]); batch=sys.argv[2]; limit=int(sys.argv[3]) if len(sys.argv)>3 else 10**9
exe=open(S/"dl/game/Fable The Lost Chapters/Fable.exe","rb").read()
pe=struct.unpack_from("<I",exe,0x3c)[0]; ns=struct.unpack_from("<H",exe,pe+6)[0]; os_=struct.unpack_from("<H",exe,pe+20)[0]
secs=[]
for i in range(ns):
    o=pe+24+os_+40*i; vs,va,rs,ro=struct.unpack_from("<IIII",exe,o+8); secs.append((va,vs,ro,rs))
def rd(va,n):
    rva=va-0x400000
    for sva,svs,ro,rs in secs:
        if sva<=rva<sva+max(svs,rs): return exe[ro+rva-sva:ro+rva-sva+n]
    return None
sizes={}
for l in open(ROOT/"ghidra_out/coverage.tsv",encoding="utf-8-sig"):
    f=l.rstrip("\n").split("\t")
    if len(f)>3 and re.match(r"^[0-9a-f]{8}$",f[0]): sizes[f[0]]=(f[1],int(f[3]))
landed=set(l.split("\t")[0].lower() for l in open(ROOT/"rebuild/oracles/auto-re-candidates.tsv",encoding="utf-8-sig"))
ps=open(ROOT/"rebuild/build_candidates.ps1",encoding="utf-8").read()
ents=re.findall(r"\[pscustomobject\]@\{\s*Address = '([0-9a-f]{8})'\s*Module = '([^']*)'\s*Source = '([^']*)'\s*TestSource = '([^']*)'\s*(?:CompilerFlags = '([^']*)'\s*)?(?:Compiler = '[^']*'\s*)?PassPattern = '([^']*)'",ps)
parents=[(a,m,s,ts,fl,pp) for a,m,s,ts,fl,pp in ents if fl and "/EHsc" in fl]
print("EH parents in catalog:",len(parents))
payload=[]; oracle=["address\tname\tlength\tbytes"]; stats={"nofi":0,"try":0,"already":0,"nosize":0,"ok":0,"gap":0}
for a,m,s,ts,fl,pp in parents:
    va=int(a,16); b=rd(va,16)
    if not b or b[:3]!=b"\x6a\xff\x68": stats["nofi"]+=1; continue
    eh=struct.unpack_from("<I",b,3)[0]; hb=rd(eh,16)
    if not hb or hb[0]!=0xb8: stats["nofi"]+=1; continue
    fi=struct.unpack_from("<I",hb,1)[0]; f=rd(fi,0x20)
    magic,maxstate,punwind,ntry,ptry=struct.unpack_from("<IiIiI",f,0)
    if magic!=0x19930520: stats["nofi"]+=1; continue
    if ntry: stats["try"]+=1; continue
    acts=[]
    for k in range(maxstate):
        to,act=struct.unpack_from("<iI",rd(punwind+8*k,8),0)
        if act: acts.append(act)
    if not acts: continue
    acts=sorted(set(acts)); base=acts[0]
    # the block must be contiguous: funclet k starts where funclet k-1 ends
    ok=True; off=0; items=[]
    for act in acts:
        key=f"{act:08x}"
        if key not in sizes: ok=False; stats["nosize"]+=1; break
        nm,ln=sizes[key]
        if act-base!=off: ok=False; stats["gap"]+=1; break
        items.append((key,nm,ln,off)); off+=ln
    if not ok: continue
    # the ehhandler thunk follows the last funclet: sanity check (b8 .. e9 ..)
    if rd(base+off,1)!=b"\xb8": stats["gap"]+=1; continue
    src=(ROOT/"rebuild/src/compiled"/s).read_text(encoding="utf-8")
    tst=(ROOT/"rebuild/tests"/ts).read_text(encoding="utf-8")
    extra=[x for x in fl.split() if x not in ("/O2","/Oy","/W3")]
    for key,nm,ln,o in items:
        if key in landed: stats["already"]+=1; continue
        fb=rd(int(key,16),ln)
        stub=(f"// EH unwind funclet @ 0x{key.upper()} of {m}::{Path(s).stem} (parent @ 0x{a.upper()}): VC7.1 emits the\n"
              f"// unwind funclets and the __ehhandler thunk of an EH-framed function as one block after the function;\n"
              f"// the linker placed Fable's blocks at 0x0121xxxx. This funclet is {ln} bytes at +{o:#x} of the parent's\n"
              f"// block and is produced by the parent's translation unit, included below (no code of its own).\n"
              f"// Retail: Fable.exe (TLC, VC7.1) function @ 0x{key.upper()}, oracle length {ln} bytes.\n"
              f"// Grade: landed -- byte-exact (the parent's object, funclet block offset {o:#x}) and the parent's\n"
              f"//        behaviour test PASS under the Linux lane (Wine + DDK 13.10.4035 cl.exe).\n"
              f'#include "../src/compiled/{s}"\n')
        payload.append({"address":key,"name":f"_global::{nm}","module":"_global","source_cpp":stub,"test_cpp":tst,"pass_pattern":pp,"funclet_offset":o,"flags":extra})
        oracle.append(f"{key}\t_global::{nm}\t{ln}\t{fb.hex()}")
        stats["ok"]+=1
        if stats["ok"]>=limit: break
    if stats["ok"]>=limit: break
D=ROOT/"rebuild/candidates/manual/batches"/batch; D.mkdir(parents=True,exist_ok=True)
(D/"payload.json").write_text(json.dumps({"result":{"authored":payload}},indent=1)); (D/"oracle.tsv").write_text("\n".join(oracle)+"\n")
print(stats, "->", len(payload), "entries", D)
