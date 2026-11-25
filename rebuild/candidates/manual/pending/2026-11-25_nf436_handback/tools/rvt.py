#!/usr/bin/env python3
"""rvt.py <va> [n]: retail vftable dwords at va (stops at a non-text dword or n), each with the self symbol / table names."""
import sys, struct
sys.path.insert(0, "/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF427/tools"); import pe
R="/home/user/FableDecomp/"
a2s={}
for l in open(R+"rebuild/oracles/self_symbols.tsv").read().splitlines()[1:]:
    f=l.split("\t")
    if len(f)>1 and f[1]: a2s[int(f[0],16)]=f[1]
t2={}
for l in open(R+"rebuild/oracles/reloc_bindings.tsv").read().splitlines()[1:]:
    f=l.split("\t")
    if len(f)>=2:
        try: t2.setdefault(int(f[1],16),[]).append(f[0])
        except: pass
def rdw(va): return struct.unpack("<I", pe.read_va(pe.EXES["retail"], va, 4))[0]
va=int(sys.argv[1],16); n=int(sys.argv[2]) if len(sys.argv)>2 else 40
col=rdw(va-4); print("COL %08x"%col)
for k in range(n):
    d=rdw(va+4*k)
    if not (0x401000<=d<0x1200000): print("end at slot",k,"%08x"%d); break
    print(k,"%08x"%d,"self=",a2s.get(d,"-"),"table=",";".join(t2.get(d,[]))[:300])
