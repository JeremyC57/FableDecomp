#!/usr/bin/env python3
"""Wave 7 item 1: a row-local NON-template `class CLevelScriptBase : public CScriptBase { Game; CScriptDataBase*
MasterScriptData; [inline ctor] };` becomes the PDB template CLevelScriptBase<T> (T = the level script's master data
class: work/lsb_T3.tsv, two sources agreeing among the Ego_r entity PMasterData types and the retail RegisterScripts
data alloc; a single source only when the landed template rows agree). The block is rewritten in the template shape
mig_sb.py accepts (the inline ctor becomes the header's: engine/CLevelScriptBase.h defines it inline), derived base
clauses and mem-initialisers name CLevelScriptBase<NScript::T>, then mig_sb.fix() replaces CScriptBase + the block with
the shared headers. If the row passes MasterScriptData where a CScriptDataBase* is wanted and T is not defined in the
row, T gets its PDB base only: `namespace NScript { class T : public CScriptDataBase { }; }`.
Rows with `CGameScriptInterface* Game` (item 2) are left. usage: lsb_nt.py [--dry] <file>..."""
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from common import find_block, norm
import mig_sb
S = os.environ["S"]
TT = {}
for l in open(S + "/mig7/work/lsb_T3.tsv"):
    s, tp, tg, tr = (l.rstrip("\n").split("\t") + ["", "", ""])[:4]
    srcs = [x for x in (tp, tg) if x and x != "(none)"]
    if tg == "(none)": continue
    if len(srcs) == 2 and srcs[0] == srcs[1] and "," not in srcs[0]: TT[s] = (srcs[0], "pdb+reg")
    elif len(srcs) == 1 and "," not in srcs[0] and tr == srcs[0]: TT[s] = (srcs[0], "one+rows")
# fallback: the row cites its script vtable; RegisterScripts registers that script's Alloc (which stores the vtable) with
# a master-data Alloc (regmap.tsv); the five data Allocs store vtables 0x012C2718/24/30/3C/48 in the order of the five
# retail CLevelScriptBase<T> vtables 0x012C2F00/14/28/3C/2EEC (bound in reloc_bindings.tsv)
DATA = {"ebp:cdbd20": "CQ_SunnyvaleMasterData", "cd4a60": "CalextestData", "cd4b80": "CDanyalTestScriptsData",
        "cd4b20": "Cjoss_test_scriptsData", "cd4ac0": "CEmptyData"}
PDBT = {}
for l in open(S + "/mig7/work/lsb_T3.tsv"):
    q = (l.rstrip("\n").split("\t") + ["", "", ""])[:4]
    PDBT[q[0]] = q[1]
VT = {}
for l in open(S + "/mig7/work/regmap.tsv"):
    q = l.rstrip("\n").split("\t"); VT[q[1]] = q[2]
def by_vtable(t, ders):
    got = set()
    for m in re.finditer(r"0x([0-9A-Fa-f]{8})", t):
        v = m.group(1).lower()
        if v in VT and VT[v] in DATA: got.add(DATA[VT[v]])
    if len(got) != 1: return None
    T = got.pop()
    for d in ders:
        if PDBT.get(d) and PDBT[d] != T: return None
    return T
def fix(t):
    b = find_block(t, "CLevelScriptBase")
    if not b: return None, "noblock"
    n = norm(t[b[0]:b[1]])
    m = re.match(r"^class CLevelScriptBase : public CScriptBase \{ public: CGameScriptInterfaceBase\* Game; CScriptDataBase\* MasterScriptData;( CLevelScriptBase\(CScriptDataBase\* (\w+), CGameScriptInterfaceBase& (\w+)\) : Game\(&\3\), MasterScriptData\(\2\) \{\})? \};$", n)
    if not m: return None, "lsb shape"
    ders = set(re.findall(r"(?:class|struct)\s+(\w+)\s*:\s*public\s+CLevelScriptBase\b(?!\s*<)", t))
    if not ders: return None, "no derived"
    ts = set(TT.get(d, (None, ""))[0] for d in ders)
    if None in ts or len(ts) != 1:
        T = by_vtable(t, ders); how = "reg-vtable"
        if not T: return None, "T unknown " + ",".join(sorted(ders))
    else:
        T = ts.pop(); how = TT[next(iter(ders))][1]
    blk = ("template <class T> class CLevelScriptBase : public CScriptBase {\npublic:\n"
           "    CGameScriptInterfaceBase* Game;\n    T* MasterScriptData;\n")
    if m.group(1): blk += "    CLevelScriptBase(CScriptDataBase* data, CGameScriptInterfaceBase& game);\n"
    blk += "};\n"
    out = t[:b[0]] + blk + t[b[1]:]
    q = "NScript::" + T
    out, k1 = re.subn(r"(public\s+)CLevelScriptBase\b(?!\s*<)", r"\1CLevelScriptBase<" + q + ">", out)
    out, k2 = re.subn(r"(?<![\w~:<])CLevelScriptBase(\s*\()", "CLevelScriptBase<" + q + r">\1", out[:]) if False else (out, 0)
    # mem-initialisers: `: CLevelScriptBase(` / `, CLevelScriptBase(` outside the (now template) block
    nb = find_block(out, "CLevelScriptBase")
    head, blk2, tail = out[:nb[0]], out[nb[0]:nb[1]], out[nb[1]:]
    tail, k2 = re.subn(r"([:,]\s*)CLevelScriptBase(\s*\()", r"\1CLevelScriptBase<" + q + r">\2", tail)
    out = head + blk2 + tail
    code = re.sub(r"//[^\n]*", "", out)
    if re.search(r"\bCLevelScriptBase\b(?!\s*<)", code[nb[1]:].replace("CLevelScriptBase<" + q + ">", "")):
        return None, "bare CLevelScriptBase use left"
    # T declaration / definition
    tdef = re.search(r"\bclass\s+" + T + r"\b\s*(?::[^{;]*)?\{", code)
    tdecl = re.search(r"\bclass\s+" + T + r"\s*;", code)
    passes = re.search(r"MasterScriptData\s*[,)]", re.sub(r"//[^\n]*", "", out[nb[1]:]))
    pre = ""
    if not tdef:
        if passes:
            if not re.search(r"\bclass\s+CScriptDataBase\s*\{", code): return None, "no local CScriptDataBase"
            pre = ("namespace NScript { class %s : public CScriptDataBase { }; }   // PDB base only (Ego_r.pdb: the master data "
                   "class derives from CScriptDataBase); its members are not used here\n" % T)
        elif not tdecl:
            pre = "namespace NScript { class %s; }\n" % T
    if pre:
        # after the local CScriptDataBase definition when there is one, else right before the template block
        cb = find_block(out, "CScriptDataBase")
        nb = find_block(out, "CLevelScriptBase")
        pos = cb[1] if cb and cb[1] <= nb[0] else nb[0]
        out = out[:pos] + pre + out[pos:]
    r, why = mig_sb.fix(out)
    if r is None: return None, "mig_sb: " + why
    return r, "ok %s T=%s (%s) bases%d init%d%s" % (why, T, how, k1, k2, " tdef" if ": public CScriptDataBase" in pre else "")
if __name__ == "__main__":
    dry = "--dry" in sys.argv
    for f in [a for a in sys.argv[1:] if not a.startswith("--")]:
        t = open(f, encoding="utf-8").read(); r, why = fix(t)
        if r is None: print("skip", f, why); continue
        if not dry: open(f, "w", encoding="utf-8", newline="\n").write(r)
        print("edit", f, why)
