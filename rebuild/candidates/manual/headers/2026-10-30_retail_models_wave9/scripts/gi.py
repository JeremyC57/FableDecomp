#!/usr/bin/env python3
"""Wave 7 item 2: a row-local CLevelScriptBase (template or not) whose Game member is `CGameScriptInterface* Game` gets the
PDB type `CGameScriptInterfaceBase* Game` (Ego_r.pdb CLevelScriptBase<T>::Game; the script calls through the interface
base, whose pure slots carry the same names and indices, engine/CGameScriptInterfaceBase.h). Calls to members only the
derived CGameScriptInterface declares (non-virtual: GetScriptSpriteBySymbol, ..., given with --cast NAME) become
`static_cast<CGameScriptInterface*>(Game)->NAME(`. Then lsb_nt.fix (non-template block) or mig_sb.fix.
usage: gi.py [--dry] [--cast NAME]... <file>..."""
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from common import find_block
import mig_sb, lsb_nt
def fix(t, casts=()):
    b = find_block(t, "CLevelScriptBase")
    if not b: return None, "noblock"
    blk = t[b[0]:b[1]]
    nb, k = re.subn(r"\bCGameScriptInterface(\s*\*\s*Game;)", r"CGameScriptInterfaceBase\1", blk)
    if k != 1: return None, "no CGSI Game"
    out = t[:b[0]] + nb + t[b[1]:]
    nc = 0
    for name in casts:
        out, n = re.subn(r"(?<![\w>.])Game\s*->\s*" + name + r"\s*\(", "static_cast<CGameScriptInterface*>(Game)->" + name + "(", out)
        nc += n
    if not re.search(r"\bclass\s+CGameScriptInterfaceBase\b", out[:b[0]]) and '#include "engine/CGameScriptInterface.h"' not in out[:b[0]]:
        out = out[:b[0]] + "class CGameScriptInterfaceBase;\n" + out[b[0]:]
    if re.search(r"^\s*class CLevelScriptBase\s*:", out, re.M):
        r, why = lsb_nt.fix(out)
    else:
        r, why = mig_sb.fix(out)
    if r is None: return None, why
    return r, why + (" cast%d" % nc if nc else "")
if __name__ == "__main__":
    a = sys.argv[1:]; dry = "--dry" in a; casts = [a[i+1] for i, x in enumerate(a) if x == "--cast"]
    fl = [x for i, x in enumerate(a) if not x.startswith("--") and (i == 0 or a[i-1] != "--cast")]
    for f in fl:
        t = open(f, encoding="utf-8").read(); r, why = fix(t, casts)
        if r is None: print("skip", f, why); continue
        if not dry: open(f, "w", encoding="utf-8", newline="\n").write(r)
        print("edit", f, why)
