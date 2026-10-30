#!/usr/bin/env python3
"""cpm_peek.py [--dry] <file>...: wave 9 CPM const-only rows. Retail calls the const GetMainPlayer QBE 0x00449960 through
the main game component: FableWin.pdb ?PeekPlayerManager@CMainGameComponent@@QBEABVCPlayerManager@@XZ (const accessor,
inline in release). Runs mig_cpm's block replacement, then gives the row-local CMainGameComponent stand-in the inline
`const CPlayerManager& PeekPlayerManager() const { return *PlayerManager; }` and respells
`g_pMainGameComponent->PlayerManager->GetMainPlayer()` as `g_pMainGameComponent->PeekPlayerManager().GetMainPlayer()`,
so the header's const overload is the one called."""
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import mig_cpm
CALL = "g_pMainGameComponent->PlayerManager->GetMainPlayer()"
def fix(t):
    if CALL not in t: return None, "no MGC call"
    m = re.search(r"((?:class|struct)\s+CMainGameComponent\b[^;{]*\{[^}]*?)(\n?[ \t]*CPlayerManager\s*\*\s*PlayerManager\s*;[^\n]*)", t)
    if not m: return None, "no local MGC PlayerManager"
    r, why = mig_cpm.fix(t)
    if r is None: return None, "cpm " + why
    m = re.search(r"((?:class|struct)\s+CMainGameComponent\b[^;{]*\{[^}]*?)(\n?[ \t]*CPlayerManager\s*\*\s*PlayerManager\s*;[^\n]*)", r)
    if not m: return None, "MGC lost"
    one_line = "\n" not in m.group(2)
    add = (" const CPlayerManager& PeekPlayerManager() const { return *PlayerManager; }" if one_line else
           "\n    const CPlayerManager& PeekPlayerManager() const { return *PlayerManager; }   // FableWin.pdb ?PeekPlayerManager@CMainGameComponent@@QBEABVCPlayerManager@@XZ")
    e = m.start(2) + re.search(r"PlayerManager\s*;", m.group(2)).end() if one_line else m.end()
    r = r[:e] + add + r[e:]
    r = r.replace(CALL, "g_pMainGameComponent->PeekPlayerManager().GetMainPlayer()")
    return r, "ok"
if __name__ == "__main__":
    dry = "--dry" in sys.argv
    for f in [a for a in sys.argv[1:] if not a.startswith("--")]:
        t = open(f, encoding="utf-8").read(); r, why = fix(t)
        if r is None: print("skip", f, why); continue
        if not dry: open(f, "w", encoding="utf-8", newline="\n").write(r)
        print("edit", f, why)
