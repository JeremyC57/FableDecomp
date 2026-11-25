#!/usr/bin/env python3
"""vt432.py (vt430 + VT_SELFMAP: the batch selfmap.tsv new selves override self_symbols)
vt430.py <wd> <rows.txt> <table.tsv>: for each row object, every emitted ??_7 COMDAT whose name is bound in <table> to a retail VA:
slot k relocation symbol -> retail address (table, else the landed self symbols) vs the retail vftable dword k.
Prints per vftable: agree / WRONG (bound to a different retail function) / unknown; RTTI-spelling check is n/a (RTTI-less vftables)."""
import sys, subprocess, glob, re, struct
sys.path.insert(0, "/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF427/tools"); import pe
wd, rl, tab = sys.argv[1:4]
s2a = {}
a2s = {}
for l in open(tab).read().splitlines()[1:]:
    s, t, w = l.split("\t"); s2a.setdefault(s, set()).add(int(t, 16))
for l in open("/home/user/FableDecomp/rebuild/oracles/self_symbols.tsv").read().splitlines()[1:]:
    f = l.split("\t")
    if len(f) > 1 and f[1]: s2a.setdefault(f[1], set()).add(int(f[0], 16)); a2s[int(f[0], 16)] = f[1]
import os
if os.environ.get("VT_SELFMAP"):
    for l in open(os.environ["VT_SELFMAP"]).read().splitlines():
        f = l.split("\t")
        if len(f) > 2:
            a_ = int(f[0], 16); o_ = a2s.get(a_)
            if o_ and o_ in s2a: s2a[o_].discard(a_)
            s2a.setdefault(f[2], set()).add(a_); a2s[a_] = f[2]
def rdw(va): return struct.unpack("<I", pe.read_va(pe.EXES["retail"], va, 4))[0]
tot = {"agree": 0, "WRONG": 0, "unknown": 0}
for a in open(rl).read().split():
    objs = glob.glob(f"{wd}/**/{a}/{a}.obj", recursive=True)
    if not objs: print(a, "NOOBJ"); continue
    o = objs[0]
    import importlib.util as _iu
    _sp = _iu.spec_from_file_location("ceq", "/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF434/tools/coffload.py"); _m = _iu.module_from_spec(_sp); _sp.loader.exec_module(_m)
    for vt, rel in _m.vftables(o):
        vas = s2a.get(vt)
        if not vas or len(vas) != 1: print(a, vt, "unbound"); continue
        va = next(iter(vas))
        res = []
        for off, sym in sorted(rel):
            k = off // 4; want = rdw(va + off)
            got = s2a.get(sym)
            if not got:
                own = a2s.get(want)
                if own and own != sym and not (sym.startswith("??_E") and own == "??_G" + sym[4:]):
                    res.append("W"); tot["WRONG"] += 1; print(a, vt, "slot", k, sym, "retail %08x self %s" % (want, own))
                else: res.append("u"); tot["unknown"] += 1
            elif want in got: res.append("a"); tot["agree"] += 1
            else: res.append("W"); tot["WRONG"] += 1; print(a, vt, "slot", k, sym, "retail %08x" % want)
        print(a, vt, "%08x" % va, "".join(res))
print("TOTAL", tot)
