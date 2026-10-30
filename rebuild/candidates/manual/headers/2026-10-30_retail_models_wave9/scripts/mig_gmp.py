#!/usr/bin/env python3
"""Wave 6: GetMainPlayer accessor rows (naming wave 10 DEFERRED, 14 rows). engine/CPlayerManager.h now declares the PDB
methods (CPlayer& GetMainPlayer() 0x00449970, ...). A row-local `struct (C)PlayerManagerMethods|CPlayerManagerView :
CPlayerManager { CPlayer* GetMainPlayer(); };` accessor goes; `static_cast<X*>(E)->GetMainPlayer()->` / `((X*)E)->
GetMainPlayer()` become `E->GetMainPlayer().` / `&E->GetMainPlayer()`. usage: mig_gmp.py [--dry] <file>..."""
import sys, re
ACC = re.compile(r"[ \t]*struct\s+(C?PlayerManager(?:Methods|View))\s*:\s*(?:public\s+)?CPlayerManager\s*\{\s*CPlayer\s*\*\s*GetMainPlayer\(\);\s*(?://[^\n]*\n\s*)?\};[^\n]*\n")
def fix(t):
    m = ACC.search(t)
    if not m: return None, "no accessor"
    X = m.group(1); out = t[:m.start()] + t[m.end():]
    out, n1 = re.subn(r"static_cast<" + X + r"\s*\*>\(\s*(\w+)\s*\)\s*->\s*GetMainPlayer\(\)\s*->", r"\1->GetMainPlayer().", out)
    out, n2 = re.subn(r"\(\(" + X + r"\s*\*\)\s*(\w+(?:\(\))?)\)\s*->\s*GetMainPlayer\(\)", r"&\1->GetMainPlayer()", out)
    if re.search(r"\b" + X + r"\b", re.sub(r"//[^\n]*", "", out)): return None, "other uses of " + X
    return out, f"ok {X} {n1}+{n2}"
if __name__ == "__main__":
    dry = "--dry" in sys.argv
    for f in [a for a in sys.argv[1:] if not a.startswith("--")]:
        t = open(f, encoding="utf-8").read(); r, why = fix(t)
        if r is None: print("skip", f, why); continue
        if not dry: open(f, "w", encoding="utf-8", newline="\n").write(r)
        print("edit", f, why)
