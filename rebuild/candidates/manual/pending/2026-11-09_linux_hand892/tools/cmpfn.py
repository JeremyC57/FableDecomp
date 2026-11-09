#!/usr/bin/env python3
"""cmpfn.py <va-hex> <len-hex> <src.cpp> <leaf> : grade any symbol of a source against retail exe bytes (checks same-TU bodies)."""
import sys, os, struct, difflib
from pathlib import Path
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
S = Path(os.environ["S"]); A = S/"w22"/"agent892"
va = int(sys.argv[1], 16); n = int(sys.argv[2], 16); src = Path(sys.argv[3]).read_text(); leaf = sys.argv[4]
exe = open(os.environ["FABLE_EXE"], "rb").read()
pe = struct.unpack_from("<I", exe, 0x3c)[0]; ns = struct.unpack_from("<H", exe, pe+6)[0]; opt = struct.unpack_from("<H", exe, pe+20)[0]
for i in range(ns):
    o = pe+24+opt+40*i; vs, sva, rs, ro = struct.unpack_from("<IIII", exe, o+8)
    if sva <= va-0x400000 < sva+rs: retail = exe[va-0x400000-sva+ro:][:n]
while retail and retail[-1] == 0xCC: retail = retail[:-1]
work = A/"wk"/"cmpfn"; work.mkdir(parents=True, exist_ok=True)
st, built, sec = V.parity_of(V.vc71(src), "cmpfn", leaf, retail, work, V.env())
print(st)
if built and not st.startswith(("MATCH", "RELOC")):
    cs = Cs(CS_ARCH_X86, CS_MODE_32)
    R = [f"{i.mnemonic} {i.op_str}" for i in cs.disasm(retail, va)]; B = [f"{i.mnemonic} {i.op_str}" for i in cs.disasm(built, va)]
    for l in difflib.unified_diff(R, B, lineterm="", n=0): print(l)
