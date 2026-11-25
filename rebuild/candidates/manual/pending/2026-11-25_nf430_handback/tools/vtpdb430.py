#!/usr/bin/env python3
"""vtpdb430.py <entries.tsv> <reloc_bindings.tsv>: per retail placeholder vftable, the table's non-placeholder ??_7 spelling(s),
the FableWin.pdb ??_7 of that class (slot run until a slot with no public name), and slot-by-slot agreement of the retail slot's
catalog name (functions.tsv module::name) with the FableWin slot's public (class + method parsed from the mangled name).
Also: retail dword after the PDB's last slot (boundary), and whether that address is itself stored as an imm32 (next vftable)."""
import sys, struct, re
sys.path.insert(0, "/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF430/tools")
sys.path.insert(0, "/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF427/tools")
import fwpub, pe
v2n, n2v = fwpub.load()
R = pe.EXES["retail"]
d, base, secs = pe.load(R); va0, sz, ro, rs = secs[0]; text = d[ro:ro + rs]; tlo, thi = base + va0, base + va0 + rs
def rdw(va):
    b = pe.read_va(R, va, 4); return struct.unpack("<I", b)[0] if len(b) == 4 else None
cat = {}
for l in open("/home/user/FableDecomp/rebuild/manifest/functions.tsv"):
    f = l.split("\t")
    if f[0] != "address": cat[int(f[0], 16)] = (f[2], f[1])
tab = {}
for l in open(sys.argv[2]).read().splitlines()[1:]:
    s, t, w = l.split("\t"); tab.setdefault(t, []).append((s, w))
def parse(m):
    """mangled -> (class path, method) roughly"""
    if not m: return ("", "")
    sp = {"??_E": "`vector deleting destructor'", "??_G": "`scalar deleting destructor'", "??1": "~", "??0": "ctor"}
    for k, v in sp.items():
        if m.startswith(k):
            body = m[len(k):].split("@@")[0].split("@")
            return ("::".join(reversed(body)), v if v != "~" else "~" + body[0])
    body = m[1:].split("@@")[0].split("@")
    return ("::".join(reversed(body[1:])), body[0])
def norm(s): return s.replace("`scalar deleting destructor'", "DD").replace("`vector deleting destructor'", "DD")
ents = [l.split("\t") for l in open(sys.argv[1]).read().splitlines()[1:]]
print("va\treal\tfw_slots\tagree\tdisagree\tretail_after\tafter_is_stored\tdetail")
for e in ents:
    va = e[2]; v = int(va, 16)
    reals = sorted({s for s, w in tab.get(va, []) if s.startswith("??_7") and "CVtable_" not in s})
    if not reals:
        print("%s\t-\t\t\t\t\t\tno real spelling in table" % va); continue
    for real in reals:
        fv = n2v.get(real)
        if fv is None:
            print("%s\t%s\tNOPDB" % (va, real)); continue
        slots = []
        i = 0
        while True:
            x = fwpub.dw(fv + 4 * i)
            if not x: break
            nm = v2n.get(fwpub.follow(x))
            if nm is None: break
            slots.append(nm); i += 1
        ag = dg = 0; det = []
        for i, nm in enumerate(slots):
            rf = rdw(v + 4 * i)
            if rf is None or not (tlo <= rf < thi): dg += 1; det.append("%d:retail-not-code" % i); continue
            mod, name = cat.get(rf, ("?", "?"))
            fc, fm = parse(nm)
            ok = norm(name) == norm(fm) or (fm.startswith("~") and name == fm)
            if ok: ag += 1
            else: dg += 1; det.append("%d:%08x %s::%s vs %s" % (i, rf, mod, name, nm))
        aft = rdw(v + 4 * len(slots))
        stored = text.find(struct.pack("<I", v + 4 * len(slots))) >= 0
        aft_code = aft is not None and tlo <= aft < thi
        print("%s\t%s\t%d\t%d\t%d\t%s\t%s\t%s" % (va, real, len(slots), ag, dg, ("%08x" % aft if aft is not None else "-") + ("(code)" if aft_code else ""), stored, " | ".join(det)[:600]))
