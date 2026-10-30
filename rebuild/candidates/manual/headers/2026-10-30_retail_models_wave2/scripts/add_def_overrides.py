#!/usr/bin/env python3
"""add_def_overrides.py <file>...: give each row-local class deriving from a shared def base (CParentDefClassBase /
CSubDefClassBase / CDefClassBase) its retail slot 18-20 overrides (Transfer / Copy / GetSizeofClass) when the class's own
mangled names are in Ego_r.pdb or FableWin.pdb (so the class is concrete, as in retail). Declarations only; idempotent."""
import sys, re, os
R = os.environ["FABLE_ROOT"]
PDB = set()
for k in ("fablewin", "egor"):
    for l in open(f"{R}/ghidra_out/{k}_pdb_names.tsv", encoding="utf-8", errors="replace"):
        q = l.rstrip("\n").split("\t")
        if len(q) >= 2: PDB.add(q[1])
OV = (("?Transfer@{0}@@UAEXAAVCPersistContext@@@Z", "virtual void Transfer(CPersistContext& context);", "Transfer"),
      ("?Copy@{0}@@UAEXPBVCDefClassBase@@@Z", "virtual void Copy(const CDefClassBase* other);", "Copy"),
      ("?GetSizeofClass@{0}@@UAEKXZ", "virtual unsigned long GetSizeofClass();", "GetSizeofClass"))
RX = re.compile(r"^((?:class|struct) (?:__declspec\(novtable\) )?(\w+) : public (?:CParentDefClassBase|CSubDefClassBase|CDefClassBase) ?\{[^\n]*\n(?:[ \t]*public:[^\n]*\n)?)", re.M)
for f in sys.argv[1:]:
    t = open(f, encoding="utf-8").read(); out = t; added = []
    for m in reversed(list(RX.finditer(t))):
        cls = m.group(2); ins = ""
        line = m.group(1)
        if "public:" not in line and not line.startswith("struct"): continue
        for mang, decl, name in OV:
            ms = mang.format(cls)
            if ms in PDB and not re.search(r"\b%s\s*\(" % name, t[m.end():m.end() + 4000].split("};")[0]):
                ins += f"    {decl:<62}// {ms}\n"; added.append(f"{cls}::{name}")
        if ins: out = out[:m.end()] + ins + out[m.end():]
    if out != t:
        open(f, "w", encoding="utf-8", newline="\n").write(out); print("edit", f, " ".join(added))
    else: print("skip", f)
