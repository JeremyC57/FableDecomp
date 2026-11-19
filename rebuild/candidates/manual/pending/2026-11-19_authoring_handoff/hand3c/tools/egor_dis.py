#!/usr/bin/env python3
"""egor_dis.py <hexva> <hexlen> : disassemble Ego_r with Ego_r.pdb names on call targets"""
import sys, struct, re
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
names={}
for l in open("/home/user/FableDecomp/ghidra_out/egor_pdb_names.tsv",encoding="utf-8",errors="replace"):
    p=l.rstrip("\n").split("\t")
    try: names[int(p[0],16)]=p[1]
    except: pass
d=open("/tmp/claude-0/egod_pdb/ego_r.exe","rb").read()
pe=struct.unpack_from("<I",d,0x3c)[0]; ns=struct.unpack_from("<H",d,pe+6)[0]; osz=struct.unpack_from("<H",d,pe+20)[0]; sh=pe+24+osz
ib=struct.unpack_from("<I",d,pe+24+28)[0]
va=int(sys.argv[1],16); n=int(sys.argv[2],16)
for i in range(ns):
    v,=struct.unpack_from("<I",d,sh+40*i+12); rs,ro=struct.unpack_from("<II",d,sh+40*i+16)
    if ib+v<=va<ib+v+rs: b=d[ro+va-ib-v:ro+va-ib-v+n]
for ins in Cs(CS_ARCH_X86,CS_MODE_32).disasm(b,va):
    c=""
    m=re.fullmatch(r"0x([0-9a-f]+)",ins.op_str)
    if ins.mnemonic=="call" and m:
        t=int(m.group(1),16); c=" ; "+names.get(t,"?")
        if t not in names:
            # follow jmp thunk
            off=None
            for i in range(ns):
                v,=struct.unpack_from("<I",d,sh+40*i+12); rs,ro=struct.unpack_from("<II",d,sh+40*i+16)
                if ib+v<=t<ib+v+rs: off=ro+t-ib-v
            if off is not None and d[off]==0xe9:
                t2=t+5+struct.unpack_from("<i",d,off+1)[0]; c=" ; "+names.get(t2,"?%x"%t2)
    print("%08x %s %s%s"%(ins.address,ins.mnemonic,ins.op_str,c[:160]))
    if ins.mnemonic=="ret": break
