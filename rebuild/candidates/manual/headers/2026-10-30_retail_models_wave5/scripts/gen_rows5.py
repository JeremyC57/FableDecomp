#!/usr/bin/env python3
"""gen_rows5.py: build $S/mig5/ROWS.tsv and REVERTED.tsv for wave 5 from the family good lists, the revert logs,
logs/mc_final.log (mcheck of the final worktree against HEAD) and the vland recheck logs (last regrade wins).
The change column is the family's description plus the extra edits read off `git diff` (casts, respellings, fixers)."""
import re, os, glob, subprocess, collections
S = os.environ["S"]; W = S + "/mig5/work"; LG = S + "/mig5/logs"; R = os.environ["FABLE_ROOT"]
def lst(n): return [l.strip()[:8] for l in open(f"{W}/{n}") if l.strip()]
files = subprocess.run(["git", "-C", R, "diff", "--name-only"], capture_output=True, text=True).stdout.split()
byaddr = {re.search(r"_([0-9a-f]{8})\.cpp$", f).group(1): f for f in files if f.startswith("rebuild/src/compiled/")}
DESC = collections.OrderedDict([
  ("BO", ("bo_good.txt bo_good2.txt bo_good3.txt", "local CBaseObject -> engine/CBaseObject.h (FABLE_RETAIL_BASE_OBJECT, vtable 0x0129A7D8); m_pIntelligentPointer -> IntelligentPointer, slot-0 stand-ins dropped")),
  ("GRO", ("gro_good.txt grx_good.txt", "local CScriptGameResourceObject{Base,ScriptedThingBase,MovieBase} -> engine headers (vtables 0x0126008C / 0x0127094C / 0x01260EF4)")),
  ("SF", ("sf_good.txt", "local CSpawnedFunc<T> / CSpawnedFunc1<T,P> -> engine/CSpawnedFunc.h / CSpawnedFunc1.h (FABLE_RETAIL_SUSPENDABLE_PROCESS)")),
  ("CITC", (None, "local NScript::CIsThingConscious -> engine/CIsThingConscious.h (vtable 0x012C2FE8); ctor / dtor bodies defined out of class inline")),
  ("CGSI-w4rev", ("cg_good.txt", "wave 4 CGSI compile failure redone: local interface-only CGameScriptInterface -> engine/CGameScriptInterface.h")),
  ("CGSI-macros", ("cgm_good.txt", "local CGameScriptInterface whose filler slots are macros (expanded and counted) -> engine/CGameScriptInterface.h")),
  ("CGSI-members", ("cgx_good.txt", "local CGameScriptInterface with data members (World .. ThingSearchTools at the header's offsets) and non-virtual leaf declarations -> engine/CGameScriptInterface.h")),
  ("CGSIB", ("cgb_good.txt", "local CGameScriptInterfaceBase (+ empty derived CGameScriptInterface) -> engine/CGameScriptInterfaceBase.h / CGameScriptInterface.h (vtable 0x0126009C)")),
])
fam = collections.defaultdict(list)
for k, (lists, d) in DESC.items():
    if lists:
        for n in lists.split():
            for a in lst(n):
                if k not in fam[a]: fam[a].append(k)
fam["00eea020"].append("CITC")
def extras(f):
    d = subprocess.run(["git", "-C", R, "diff", "-U0", "--", f], capture_output=True, text=True).stdout
    add = "\n".join(l for l in d.splitlines() if l.startswith("+") and not l.startswith("+++"))
    rem = "\n".join(l for l in d.splitlines() if l.startswith("-") and not l.startswith("---"))
    x = []
    if re.search(r"\(E[A-Z]\w+\)\s*-?\w", add) and not re.search(r"\(E[A-Z]\w+\)\s*-?\w", rem): x.append("int arguments cast to the PDB enum")
    if "IsValid(" in add and "IsScripting" in rem: x.append("IsScripting() -> IsValid() const (0x00CD23B9)")
    if "template <class T> class CCountedPointer" in add: x.append("row-local CCountedPointer made the class template")
    if "fable_nscript_forward.h\"   // the PDB enums" in add: x.append("typedef enum stand-ins -> fable_nscript_forward.h")
    if re.search(r"namespace std", add) and "vector" in add: x.append("local vector moved into std (STLport arity)")
    if re.search(r"_Retail\b", rem) and re.search(r"\bclass C(PlayerManager|World|DisplayEngine|MainGameComponent|DefinitionManager)\b", add): x.append("X_Retail stand-in respelled to the header's member class")
    if "class CGameDefinitionManager : public CDefinitionManager" in add: x.append("CGameDefinitionManager : CDefinitionManager added")
    if re.search(r"CGameScriptInterface::\w+\(", add) and re.search(r"CGameScriptInterface::\w+\(", rem): x.append("leaf definition respelled to the header (PDB) signature")
    if "KillAllThingsInVector" in add or "AreAllThingsInVectorDead" in add: x.append("vector helper respelled to the PDB reference signature")
    if "(EScriptEntityMoveType)" in add: x.append("MoveToPosition move type cast")
    return x
mc = {}
if os.path.exists(LG + "/mc_final.log"):
    for l in open(LG + "/mc_final.log"):
        q = l.rstrip("\n").split("\t")
        if len(q) >= 2 and re.fullmatch(r"[0-9a-f]{8}", q[0]): mc[q[0]] = q[1:]
rg = {}
for p in sorted(glob.glob(LG + "/recheck*.log"), key=lambda p: int(re.search(r"(\d+)\.log$", p).group(1))):
    for l in open(p):
        m = re.match(r"^([0-9a-f]{8}) +(MATCH|RELOCATION_MATCH|[A-Z_]+) +(PASS|FAIL\S*)", l)
        if m: rg[m.group(1)] = m.group(2) + " " + m.group(3)
out = ["addr\tfile\tfamily\tchange\tgrade_before(HEAD, mcheck)\tgrade_after(vland --recheck-landed)\tmcheck_vs_HEAD"]
nofam = []
for a, f in sorted(byaddr.items()):
    fs = fam.get(a) or []
    if not fs: nofam.append(a)
    ch = "; then ".join(DESC[k][1] for k in fs) if fs else "?"
    ex = extras(f)
    if ex: ch += "; " + "; ".join(ex)
    m = mc.get(a, ["?", ""])
    g = re.search(r"\b(MATCH|RELOCATION_MATCH|[A-Z_]*MISMATCH|[A-Z_]*FAIL)\b", m[1]) if len(m) > 1 else None
    before = g.group(1) if g else ("graded (leaf respelled)" if len(m) > 1 and "leafsym" in m[1] else "?")
    out.append(f"{a}\t{f}\t{'+'.join(fs) or '?'}\t{ch}\t{before}\t{rg.get(a, 'NOT REGRADED')}\t{m[0]}")
open(S + "/mig5/ROWS.tsv", "w").write("\n".join(out) + "\n")
rv = ["addr\tfamily\treason"]
seen = set()
for n in ("bo_rev.tsv", "gro_rev.tsv", "sf_rev.tsv", "cg_rev.tsv", "cgm_rev.tsv", "cgx_rev.tsv", "grx_rev.tsv", "cgb_rev.tsv"):
    for l in open(f"{W}/{n}"):
        q = l.rstrip("\n").split("\t")
        if len(q) < 3 or q[0] == "addr": continue
        key = (q[0], q[1])
        if key in seen: continue
        seen.add(key)
        fk = {"GRO-cp": "GRO", "CGSI-members": "CGSI-members", "CGSI-macro": "CGSI-macros", "CGSIB": "CGSIB"}.get(q[1], q[1].split("-")[0])
        later = [k for k in fam.get(q[0], []) if k == fk or k.startswith(fk)]
        state = (" [migrated later in this wave: " + "+".join(fam[q[0]]) + "]") if later else \
                (" [row keeps an earlier wave 5 migration: " + "+".join(fam.get(q[0], [])) + "]") if q[0] in byaddr else ""
        rv.append(f"{q[0]}\t{q[1]}\t{q[2]}{state}")
open(S + "/mig5/REVERTED.tsv", "w").write("\n".join(rv) + "\n")
c = collections.Counter("+".join(fam.get(a) or ["?"]) for a in byaddr)
print(len(byaddr), "edited rows;", len(rv) - 1, "reverted entries; no family:", nofam[:20])
for k, v in c.most_common(): print(v, k)
print(collections.Counter(r.split("\t")[5] for r in out[1:]))
print(collections.Counter(r.split("\t")[6] for r in out[1:]))
