#!/usr/bin/env python3
"""Wave 8 family CPM: rows whose local `class CPlayerManager { ... };` only declares accessor methods that
engine/CPlayerManager.h declares with the same signature (the five Ego_r.pdb / FableWin.pdb accessors: GetPlayer QAE
0x004498C0 / QBE 0x00449910, GetMainPlayer QAE 0x00449970 / QBE 0x00449960, GetMainPlayerNumber QBE 0x00449700)
move onto the shared header: the block is replaced by the include (at the block's place, or above the outermost
namespace that holds it). Rows that declare `struct` CPlayer / CMainGameComponent / CGameDefinitionManager are skipped
(the header forward-declares them as class).
usage: mig_cpm.py [--dry] <file>..."""
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from nsutil import mask, top_insert_pos
OK = {"CPlayer&GetPlayer(long#);", "constCPlayer&GetPlayer(long#)const;", "CPlayer&GetMainPlayer();",
      "constCPlayer&GetMainPlayer()const;", "longGetMainPlayerNumber()const;"}
def fix(t):
    if '#include "engine/CPlayerManager.h"' in t: return None, "already"
    mt = mask(t)
    ms = list(re.finditer(r"^[ \t]*class\s+CPlayerManager\s*\{", mt, re.M))
    if len(ms) != 1: return None, "blocks %d" % len(ms)
    if len(re.findall(r"\b(class|struct)\s+CPlayerManager\b", mt)) != 1 + len(re.findall(r"\b(class|struct)\s+CPlayerManager\s*;", mt)): return None, "other decl"
    m = ms[0]; i = mt.index("{", m.start()); j = mt.index("}", i)
    if "{" in mt[i + 1:j]: return None, "bodies"
    k = j + 1
    while mt[k] in " \t": k += 1
    if mt[k] != ";": return None, "noterm"
    k += 1
    if k < len(t) and t[k] == "\n": k += 1
    body = re.sub(r"\b(public)\s*:", "", mt[i + 1:j])
    for d in [x.strip() for x in body.split(";") if x.strip()]:
        n = re.sub(r"\s+", "", d) + ";"
        n = re.sub(r"\(long\w+\)", "(long#)", n)
        if n not in OK: return None, "member " + d[:60]
    for c in ("CPlayer", "CMainGameComponent", "CGameDefinitionManager", "CPlayerManager"):
        if re.search(r"\bstruct\s+" + c + r"\b", mt): return None, "struct " + c
        if re.search(r"\btypedef\b[^;]*\b" + c + r"\s*;", mt): return None, "typedef " + c
        if re.search(r"\bnamespace\b[^{;]*\{[^}]*\bclass\s+" + c + r"\b", mt): return None, "nsdecl " + c
    ins = top_insert_pos(t, m.start())
    if ins is None: return None, "nested"
    inc = ("// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared\n"
           "//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,\n"
           "//   GetMainPlayerNumber 0x00449700).\n"
           '#include "engine/CPlayerManager.h"\n')
    if ins == m.start(): out = t[:m.start()] + inc + t[k:]
    else: out = t[:ins] + inc + t[ins:m.start()] + t[k:]
    return out, "ok"
if __name__ == "__main__":
    dry = "--dry" in sys.argv
    for f in [a for a in sys.argv[1:] if not a.startswith("--")]:
        t = open(f, encoding="utf-8").read(); r, why = fix(t)
        if r is None: print("skip", f, why); continue
        if not dry: open(f, "w", encoding="utf-8", newline="\n").write(r)
        print("edit", f, why)
