#!/usr/bin/env python3
"""dumpobj.py <addr> <leaf> : disassemble the last g.py build of <addr> (tmp/q/<addr>/<addr>.obj)"""
import sys, os
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
a=sys.argv[1]; leaf=sys.argv[2]
p=os.path.join(os.environ["W"],"tmp","q",a,a+".obj")
r=V.obj_text(p,leaf); b=r[0] if isinstance(r,tuple) else r
for i in Cs(CS_ARCH_X86,CS_MODE_32).disasm(bytes(b),int(a,16)): print("%08x %s %s"%(i.address,i.mnemonic,i.op_str))
