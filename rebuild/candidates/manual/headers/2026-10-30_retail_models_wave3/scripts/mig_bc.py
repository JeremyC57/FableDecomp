#!/usr/bin/env python3
"""Wave 1 family BC: a row-local trivial CBaseClass (any mix of `CBaseClass();`, `CBaseClass() { }`, `virtual ~CBaseClass();`,
`virtual ~CBaseClass() { }`, `CBaseClass(const CBaseClass&) { }`) becomes the shared engine/CBaseClass.h (RTTI root, vtable
0x01231710: CBaseClass() 0x0099A2D0, virtual ~CBaseClass 0x0099A2E0, implicit copy). Rows that name a stand-in slot (`v00`)
are skipped. Idempotent (a file without the local block is left alone).
usage: mig_bc.py [--dry] <file>..."""
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from common import find_block, norm
OK = re.compile(r"^(?:class|struct) CBaseClass ?\{ ?public: ?(?:CBaseClass\(\); ?|CBaseClass\(\) ?\{ ?\} ?|virtual ?~CBaseClass\(\); ?"
                r"|virtual ?~CBaseClass\(\) ?\{ ?\} ?|CBaseClass\(const CBaseClass ?& ?(?:other)?\) ?\{ ?\} ?)+\};$")
REPL = '#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0\n'
def fix(t):
    b = find_block(t, "CBaseClass")
    if not b: return None, "noblock"
    n = norm(t[b[0]:b[1]])
    if not OK.match(n): return None, "shape"
    if '#include "engine/CBaseClass.h"' in t: return None, "already includes"
    if re.search(r"\bv00\b", t) and not ALLOW_V00: return None, "v00 stand-in"
    return t[:b[0]] + REPL + t[b[1]:], "ok"
ALLOW_V00 = "--allow-v00" in sys.argv   # wave 2: a v00 stand-in in another class does not depend on CBaseClass (mcheck decides)
if __name__ == "__main__":
    dry = "--dry" in sys.argv
    for f in [a for a in sys.argv[1:] if not a.startswith("--")]:
        t = open(f, encoding="utf-8").read()
        r, why = fix(t)
        if r is None: print("skip", f, why); continue
        if not dry: open(f, "w", encoding="utf-8", newline="\n").write(r)
        print("edit", f, why)
