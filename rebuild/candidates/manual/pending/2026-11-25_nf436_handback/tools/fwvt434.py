#!/usr/bin/env python3
"""fwvt434.py <class-mangled-scope>...: FableWin.pdb ??_7<scope>@@6B@ slot names (slot run until a slot with no public name or a COL),
plus the retail vftable (table binding) slot addresses and their landed selves. scope e.g. CGlobal_TeleportToHeroGuildScript@NScript"""
import sys, struct
sys.path.insert(0, "/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF436/tools")
sys.path.insert(0, "/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF427/tools")
import fwpub, pe
v2n, n2v = fwpub.load()
R = pe.EXES["retail"]
def rdw(va): return struct.unpack("<I", pe.read_va(R, va, 4))[0]
tab = {}
for l in open("/home/user/FableDecomp/rebuild/oracles/reloc_bindings.tsv").read().splitlines()[1:]:
    s, t, w = l.split("\t"); tab.setdefault(s, set()).add(int(t, 16))
selves = {}
for l in open("/home/user/FableDecomp/rebuild/oracles/self_symbols.tsv").read().splitlines()[1:]:
    f = l.split("\t")
    if len(f) > 1: selves[int(f[0], 16)] = f[1]
def fwslots(vt):
    fv = n2v.get(vt)
    if fv is None: return None
    out = []
    for i in range(80):
        x = fwpub.dw(fv + 4 * i)
        if not x: break
        nm = v2n.get(fwpub.follow(x))
        if nm is None or nm.startswith("??_R4"): break
        if i > 0 and any(k.startswith("??_7") for k in [v2n.get(fv + 4 * i, "")]): break
        out.append(nm)
    return out
for sc in sys.argv[1:]:
    vt = "??_7%s@@6B@" % sc
    fs = fwslots(vt)
    rv = tab.get(vt)
    print("##", vt, "fw", None if fs is None else len(fs), "retail", ",".join("%08x" % v for v in sorted(rv or [])))
    if fs is None: continue
    for k, nm in enumerate(fs):
        ra = rdw(next(iter(rv)) + 4 * k) if rv and len(rv) == 1 else 0
        print("%d\t%s\t%08x\t%s" % (k, nm, ra, selves.get(ra, "-")))
