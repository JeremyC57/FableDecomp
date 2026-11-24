#!/usr/bin/env python3
"""sdiff.py <obj> <addr> <leaf>: capstone listing of retail (catalog oracle) v the object's leaf function, side by side diff."""
import sys, os, difflib, contextlib, io
sys.path.insert(0, '/home/user/FableDecomp/tools/decomp_pipeline'); sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import verify_and_land as V, nf
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
o, a, leaf = sys.argv[1:4]
r = bytes.fromhex(nf.catalog()[a]['bytes'])
with contextlib.redirect_stderr(io.StringIO()): b = V.obj_text(o, leaf)[0]
md = Cs(CS_ARCH_X86, CS_MODE_32)
f = lambda x: ['%s %s' % (i.mnemonic, i.op_str) for i in md.disasm(x, 0)]
for l in difflib.unified_diff(f(r), f(b), 'retail', 'built', lineterm='', n=40): print(l)
