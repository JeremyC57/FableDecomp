#!/usr/bin/env python3
"""Generate rebuild/include/engine/CGameScriptInterfaceBase.h (FABLE_RETAIL_NSCRIPT) from ghidra_out/cgsi_slots_aligned.tsv
(retail CGameScriptInterface vtable 0x01260F0C, slot -> address -> FableWin/Ego_r mangled name) and the VC7.1 undname
spelling of each mangled name. `order.json` (optional) gives the declaration order per overload group (MSVC lays overloads
out in reverse declaration order); the probe (probe_cgsi.py) checks the emitted vtable against retail.
usage: gen_cgsi.py <out.h> [--probe <probe.cpp>]"""
import sys, os, re, json, csv
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from und2decl import decl
S = os.environ["S"]; R = os.environ["FABLE_ROOT"]
rows = [r for r in csv.reader(open(R + "/ghidra_out/cgsi_slots_aligned.tsv"), delimiter="\t") if r and not r[0].startswith("#")]
U = open(S + "/mig6/work/cgsi_undname.txt").read().splitlines()
M = open(S + "/mig6/work/cgsi_mangled.txt").read().splitlines()
und = dict(zip(M, U))
egor = set(l.rstrip("\n").split("\t")[1] for l in open(R + "/ghidra_out/egor_pdb_names.tsv", errors="replace") if "\t" in l)
slots = []   # (slot, addr, mangled, decl)
for r in rows:
    i = int(r[0])
    if not r[3].startswith("?"): continue
    acc, d = decl(und[r[3]], "CGameScriptInterface", pure=False)
    slots.append((i, r[1], r[3], d, acc))
base = [s for s in slots if s[0] <= 916]; derived_only = [s for s in slots if s[0] > 916]
# nested / unforwardable parameter types: keep the slot and its name, use an ABI-identical stand-in, say so
SUBST = {"CHeroLogBook::CHeroLogBookEntry::ECategory": "long"}
NOTE = {}
def fix(d):
    for k, v in SUBST.items():
        if k in d: d = d.replace(k, v); NOTE[d] = k
    return d
# overload groups: names declared more than once; MSVC emits each group contiguously, in reverse declaration order
order_file = S + "/mig6/work/cgsi_order.json"
rev = json.load(open(order_file)) if os.path.exists(order_file) else {}
def name_of(d): return re.search(r"(\w+)\(", d).group(1)
def emit(sl, pure):
    groups = {}; out = []; seen = set()
    for s in sl: groups.setdefault(name_of(s[3]), []).append(s)
    for s in sl:
        n = name_of(s[3])
        if n in seen: continue
        seen.add(n)
        g = groups[n]
        if len(g) > 1 and rev.get(n, "reverse") == "reverse": g = list(reversed(g))
        for x in g:
            d = fix(x[3])
            if pure: d = d[:-1] + " = 0;"
            if x[4] != "public": d = f"{x[4]}: {d} public:"
            out.append((x, d))
    return out
def header():
    L = []
    L.append("#ifndef FABLE_ENGINE_CGAMESCRIPTINTERFACEBASE_H")
    L.append("#define FABLE_ENGINE_CGAMESCRIPTINTERFACEBASE_H")
    L.append("// Retail polymorphic interface (hand-generated 2026-10-29 by the wave 1 class-model migration, scripts/gen_cgsi.py).")
    L.append("// RTTI .?AVCGameScriptInterfaceBase@@ : CBaseClassNonCopyable : CBaseClass (ghidra_out/rtti_bases.tsv); own vtable 0x0126009C")
    L.append("// = slot 0 scalar deleting dtor 0x006E7180 + 916 _purecall (0x00BFEA08) slots. Every slot is named from the overriding")
    L.append("// CGameScriptInterface (RTTI, vtable 0x01260F0C; ghidra_out/cgsi_slots_aligned.tsv): the mangled names are FableWin.pdb's")
    L.append("// (916 of them also in Ego_r.pdb; slot 2 TraceMessage is FableWin-only) and each declaration below is the VC7.1 undname")
    L.append("// spelling with the class prefix dropped. Overloads are declared in reverse vtable order (VC7.1 groups them, last declared")
    L.append("// first); the probe build of this header reproduces all 917 mangled names in retail slot order (HEADERS.md).")
    L.append("// Slot 378 EndLetterBox is protected (FableWin ?EndLetterBox@CGameScriptInterface@@MAEXXZ). One stand-in: slot 308 AddLogBookEntry takes CHeroLogBook::CHeroLogBookEntry::ECategory (a nested enum that cannot be")
    L.append("// forward-declared); it is declared `long` here (same slot, same 4-byte by-value ABI; its mangled name differs).")
    L.append("// Enums are MS forward declarations (VC7.1 extension); under g++ (C++98 has no opaque enums) they get empty placeholder")
    L.append("// definitions so the header still parses (the landed rows are only ever compiled by VC7.1).")
    L.append("#ifndef FABLE_RETAIL_NSCRIPT")
    L.append("#error engine/CGameScriptInterfaceBase.h is the retail NScript model: define FABLE_RETAIL_NSCRIPT before including it")
    L.append("#else")
    L.append('#include "rebuild_abi.h"')
    L.append('#include "engine/CBaseClassNonCopyable.h"')
    L.append('#include "engine/fable_nscript_forward.h"')
    L.append("")
    L.append("class CGameScriptInterfaceBase : public CBaseClassNonCopyable {")
    L.append("public:")
    L.append("    virtual ~CGameScriptInterfaceBase();                                 // slot 0  0x006E7180 (scalar deleting)")
    for (s, d) in emit(base, True):
        L.append(f"    {d:<100} // {s[0]:3} 0x{s[1].upper()}")
    L.append("};")
    L.append("FABLE_STATIC_ASSERT(sizeof(CGameScriptInterfaceBase) == 0x4);")
    L.append("")
    L.append("#endif // FABLE_RETAIL_NSCRIPT")
    L.append("#endif // FABLE_ENGINE_CGAMESCRIPTINTERFACEBASE_H")
    return "\n".join(L) + "\n"
if __name__ == "__main__":
    out = sys.argv[1]
    open(out, "w").write(header())
    if "--probe" in sys.argv:
        p = sys.argv[sys.argv.index("--probe") + 1]
        P = ["#define FABLE_RETAIL_NSCRIPT", '#include "engine/CGameScriptInterfaceBase.h"',
             "class CGameScriptInterface : public CGameScriptInterfaceBase {", "public:", "    CGameScriptInterface();",
             "    virtual ~CGameScriptInterface();"]
        for (s, d) in emit(base + derived_only, False): P.append("    " + d)
        P += ["};", "CGameScriptInterface::CGameScriptInterface() { }"]
        open(p, "w").write("\n".join(P) + "\n")
