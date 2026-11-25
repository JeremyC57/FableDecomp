#!/usr/bin/env python3
"""xref430.py: for each vftable VA (from vt_col.tsv col1), find .text sites holding the imm32 and the enclosing catalog function."""
import sys, struct, bisect
sys.path.insert(0, "/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF427/tools")
import pe
d, base, secs = pe.load(pe.EXES["retail"])
va0, sz, ro, rs = secs[0]
text = d[ro:ro + rs]
tbase = base + va0
funcs = []
for l in open("/home/user/FableDecomp/rebuild/manifest/functions.tsv"):
    f = l.split("\t")
    if f[0] == "address": continue
    funcs.append((int(f[0], 16), f[1], f[2]))
funcs.sort()
fa = [x[0] for x in funcs]
def owner(a):
    i = bisect.bisect_right(fa, a) - 1
    return funcs[i] if i >= 0 else (0, "", "")
want = [int(l.split("\t")[0], 16) for l in open(sys.argv[1])]
for v in want:
    pat = struct.pack("<I", v)
    sites = []
    i = text.find(pat)
    while i >= 0:
        a = tbase + i
        o = owner(a)
        # store pattern: preceding byte C7 (mov r/m32, imm32)
        op = text[i-2:i]
        sites.append("%08x|%08x|%s::%s|%s" % (a, o[0], o[2], o[1].replace(" ","_"), op.hex()))
        i = text.find(pat, i + 1)
    print("%08x\t%s" % (v, " ".join(sites)))
