#!/usr/bin/env python3
"""gen_rows6.py: write $S/mig9/ROWS.tsv and REVERTED.tsv from the wave 6 work files (edit notes, mcheck statuses,
recheck logs). Columns: addr, file, family, change, grade_before (HEAD, mcheck), grade_after (vland), mcheck_vs_HEAD."""
import os, re, subprocess, glob
S = os.environ["S"]; R = os.environ["FABLE_ROOT"]; W = S + "/mig9/work"; Lg = S + "/mig9/logs"
files = subprocess.run(["git", "-C", R, "diff", "--name-only"], capture_output=True, text=True).stdout.split()
rows = {re.search(r"_([0-9a-f]{8})\.cpp$", f).group(1): f for f in files if f.startswith("rebuild/src/compiled/") and re.search(r"_([0-9a-f]{8})\.cpp$", f)}
def addr(p): m = re.search(r"_([0-9a-f]{8})\.cpp", p); return m.group(1) if m else None
fam = {}; chg = {}
def add(a, f, c):
    fam.setdefault(a, []); chg.setdefault(a, [])
    if f not in fam[a]: fam[a].append(f)
    if c and c not in chg[a]: chg[a].append(c)
for l in open(W + "/gmp_files.txt"): add(addr(l), "GMP", "row-local GetMainPlayer accessor class -> engine/CPlayerManager.h GetMainPlayer() 0x00449970 (CPlayer&)")
for l in open(Lg + "/cgsi_a_apply.txt"):
    q = l.split(" ", 3)
    if q[0] != "edit": continue
    note = q[3].strip() if len(q) > 3 else "ok"
    parts = ["local CGameScriptInterface -> engine/CGameScriptInterface.h (FABLE_RETAIL_NSCRIPT)"]
    if "alias" in note: parts.append("HeaderLayout/Donor alias trio dropped")
    if "sprite" in note: parts.append("GetTextIDFromSymbol -> GetScriptSpriteBySymbol 0x006E77F0")
    if "acc:" in note: parts.append("inline member accessor dropped (" + ",".join(re.findall(r"acc:(\w+?)\d+", note)) + ")")
    if "DefMgr" in note or "CWorld" in note or "CMainGameComponent" in note: parts.append("X_Retail member type respelled")
    add(addr(q[1]), "CGSI", "; ".join(parts))
for f, c in (("c3857_files.txt", "cpfix: CCountedPointer made the template"), ("fs.txt", "float setter: PDB float parameter, float store target")):
    for l in open(W + "/" + f): add(addr(l), "CGSI", c)
add("008a4a00", "CGSI", "float setter: PDB float parameter, float store target")
for a in open(W + "/sig.txt").read().split(): add(a, "CGSI", "sigfix: definition head = the PDB signature")
for fn in ("gro_full_dry.txt", "gro_b.txt"):
    for l in open(W + "/" + fn):
        q = l.split(" ", 3)
        if q[0] != "edit": continue
        cp = re.search(r"cp=(\S+)", l); cp = cp.group(1) if cp else ""
        add(addr(q[1]), "GRO", "local GRO classes -> CScriptGameResourceObject{Base,ScriptedThingBase}.h; full retail CCountedPointer<T> (release dtor) replaces the row's " + {"words": "two-word CCountedPointerWords", "tmpl": "dtor-less template", "none": "missing pointer model", "has": "own template"}.get(cp, cp))
for l in open(W + "/sb_dry.txt"):
    q = l.split(" ", 3)
    if q[0] != "edit": continue
    note = q[3].strip()
    c = "local CScriptBase" + (" + CLevelScriptBase<T>" if "lsb" in note else "") + " -> engine/" + ("CLevelScriptBase.h" if "lsb" in note else "CScriptBase.h") + " (FABLE_RETAIL_NSCRIPT, vtable 0x012C1648)"
    if "spawn" in note: c += "; SpawnFunction helper -> AddSpawnedFunction(f, CCharString(r)) 0x00CB7E50"
    add(addr(q[1]), "SB", c)
def okset(*fs):
    s = set()
    for f in fs:
        for l in open(W + "/" + f):
            q = l.rstrip("\n").split("\t")
            if len(q) > 1 and q[1] in ("SAME", "RELOC_RENAMED"): s.add(q[0])
    return s
keep = {"CGSI": okset("st_cgsi_a.tsv"), "GRO": okset("st_gro_a.tsv", "st_gro_b.tsv"), "SB": set(open(W + "/sb_ok.txt").read().split())}
for a in list(fam):
    pairs = [(f, c) for f, c in zip(fam[a], fam[a])]
    fam[a] = [f for f in fam[a] if f not in keep or a in keep[f]]
    chg[a] = [c for c in chg[a] if not (c.startswith("local GRO") and a not in keep["GRO"]) and not ((c.startswith("local CGameScriptInterface") or c.startswith("cpfix") or c.startswith("float") or c.startswith("sigfix")) and a not in keep["CGSI"]) and not (c.startswith("local CScriptBase") and a not in keep["SB"])]
st = {}
for l in open(W + "/st_all.tsv"):
    a, s, d = (l.rstrip("\n").split("\t") + ["", ""])[:3]; st[a] = (s, d)
after = {}
for p in sorted(glob.glob(Lg + "/recheck*.log"), key=lambda p: int(re.search(r"recheck(\d+)", p).group(1))):
    seen = set()
    for l in open(p):
        m = re.match(r"^([0-9a-f]{8})\s+(\S+)\s+(\S+)\s", l)
        if not m or m.group(2) == "WARN" or m.group(1) in seen: continue
        seen.add(m.group(1)); after[m.group(1)] = m.group(2) + " " + m.group(3)
with open(S + "/mig9/ROWS.tsv", "w") as o:
    o.write("addr\tfile\tfamily\tchange\tgrade_before(HEAD, mcheck)\tgrade_after(vland --recheck-landed)\tmcheck_vs_HEAD\n")
    for a, f in sorted(rows.items()):
        s, d = st.get(a, ("?", ""))
        m = re.search(r"(MATCH|RELOCATION_MATCH)->", d) or re.match(r"(MATCH|RELOCATION_MATCH)\b", d)
        before = m.group(1) if m else after.get(a, "?").split()[0] + " (same bytes)"
        o.write("\t".join([a, f, "+".join(fam.get(a, ["?"])), "; ".join(chg.get(a, [])), before, after.get(a, "NOT_REGRADED"), s + (" " + d[:160] if s == "RELOC_RENAMED" else "")]) + "\n")
with open(S + "/mig9/REVERTED.tsv", "w") as o:
    o.write("addr\tfamily\treason\n")
    for l in open(W + "/rev.tsv"): o.write(l)
print(len(rows), "rows")
