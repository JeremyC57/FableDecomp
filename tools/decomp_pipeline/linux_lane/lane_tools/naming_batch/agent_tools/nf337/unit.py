#!/usr/bin/env python3
"""unit.py (NF337) <run> <copyaddr:class:opaddr[:opsym]>...: per Copy-thunk unit, from work/<run>/{head,new}/wd objects:
  - HEAD Copy object: exactly one DEF (the flat stand-in) and one UND (the made-up impl);
  - new Copy object: DEF == {?Copy@C@@UAEXPBVCDefClassBase@@@Z}, UND == {opsym} (default ??4C@@QAEAAV0@ABV0@@Z), no ??_7 / ??_R;
  - objcheck HEAD v new under the explicit two-entry map (self, impl -> opsym) = ALL_EQ / LIBCP;
  - the operator= row's HEAD object defines opsym (selection: strong or pick-any COMDAT), printed.
Writes work/<run>/UNITS.tsv; prints it."""
import os, sys, struct
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import drv
S = os.environ["S"]; W = f"{S}/w22/agentNF337/work"
def obj(run, side, a): return f"{W}/{run}/{side}/wd/{a[:2]}/{a[2:4]}/{a}/{a}.obj"
def comdat_sel(path, sym):
    d = open(path, "rb").read(); nsec, = struct.unpack_from("<H", d, 2); symp, nsym = struct.unpack_from("<II", d, 8); strt = symp + nsym * 18
    def nm(e):
        if e[:4] == b"\0\0\0\0":
            o, = struct.unpack_from("<I", e, 4); return d[strt + o:d.index(b"\0", strt + o)].decode("latin-1")
        return e[:8].rstrip(b"\0").decode("latin-1")
    i = 0; secaux = {}; found = None
    while i < nsym:
        e = d[symp + i * 18:symp + i * 18 + 18]; name = nm(e); val, sec, typ, cl, aux = struct.unpack_from("<IhHBB", e, 8)
        if cl == 3 and aux and name.startswith(".text"):
            a = d[symp + (i + 1) * 18:symp + (i + 1) * 18 + 18]; secaux[sec] = a[14]   # Selection byte of the section-definition aux record
        if cl == 2 and name == sym and sec > 0: found = sec
        i += 1 + aux
    if found is None: return "ABSENT"
    return {1: "NODUPLICATES(strong)", 2: "ANY(pick-any)", 3: "SAME_SIZE", 4: "EXACT_MATCH", 5: "ASSOCIATIVE", 6: "LARGEST"}.get(secaux.get(found), "noncomdat")
run = sys.argv[1]; out = []
for spec in sys.argv[2:]:
    p = spec.split(":"); a, C, op = p[0], p[1], p[2]
    opsym = p[3] if len(p) > 3 else f"??4{C}@@QAEAAV0@ABV0@@Z"; pdb = f"?Copy@{C}@@UAEXPBVCDefClassBase@@@Z"
    oh, on, oo = obj(run, "head", a), obj(run, "new", a), obj(run, "head", op)
    _, dh, uh = drv.coff(oh); _, dn, un = drv.coff(on)
    v = []
    if len(dh) != 1 or len(uh) != 1: v.append(f"HEAD symbols DEF={sorted(dh)} UND={sorted(uh)}")
    if dn != {pdb}: v.append(f"new DEF={sorted(dn)}")
    if un != {opsym}: v.append(f"new UND={sorted(un)}")
    mp = {next(iter(dh)): pdb, next(iter(uh)): opsym} if len(dh) == 1 and len(uh) == 1 else {}
    oc = drv.objcheck(oh, on, mp)
    if oc not in ("ALL_EQ", "LIBCP"): v.append("objcheck " + oc)
    sel = comdat_sel(oo, opsym) if os.path.exists(oo) else "NO_OBJ"
    if sel == "ABSENT": v.append(f"operator= row {op} does not define {opsym}")
    out.append("\t".join([a, C, op, "OK" if not v else "FAIL", oc, next(iter(dh)) if len(dh) == 1 else "?", next(iter(uh)) if len(uh) == 1 else "?", opsym, sel, "; ".join(v)]))
open(f"{W}/{run}/UNITS.tsv", "w").write("copy\tclass\toperator=\tverdict\tobjcheck\thead_self\thead_callee\tnew_callee\toperator=_definition\tnotes\n" + "\n".join(out) + "\n")
print("\n".join(out))
