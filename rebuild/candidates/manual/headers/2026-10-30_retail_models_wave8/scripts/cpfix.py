#!/usr/bin/env python3
"""cpfix.py [--dry] <file>...: a row-local NON-template `struct CCountedPointer { ... };` used only as the CScriptThing
member `CCountedPointer PImp;` clashes with fable_nscript_forward.h's `template <class T> class CCountedPointer;`. Make it
the template the retail type is (Ego_r.pdb: CScriptThing::PImp is CCountedPointer<CScriptThing>): `template <class T> class
CCountedPointer { public: ... };` and `CCountedPointer<CScriptThing> PImp;`. All its members are inline (no relocation)."""
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from common import find_block
def fix(t):
    m = re.search(r"(?m)^struct CCountedPointer \{", t)
    if not m: return None, "no non-template struct"
    b = find_block(t, "CCountedPointer")
    if not b or b[0] != m.start(): return None, "block"
    rest = t[b[1]:]
    uses = re.findall(r"\bCCountedPointer\b(?!<)", re.sub(r"//[^\n]*", "", rest))
    if len(uses) != len(re.findall(r"\bCCountedPointer PImp;", rest)): return None, "other uses"
    blk = t[b[0]:b[1]].replace("struct CCountedPointer {", "template <class T> class CCountedPointer { public:", 1)
    rest = rest.replace("CCountedPointer PImp;", "CCountedPointer<CScriptThing> PImp;")
    return t[:b[0]] + blk + rest, "ok"
if __name__ == "__main__":
    dry = "--dry" in sys.argv
    for f in [a for a in sys.argv[1:] if not a.startswith("--")]:
        t = open(f, encoding="utf-8").read(); r, why = fix(t)
        if r is None: print("skip", f, why); continue
        if not dry: open(f, "w", encoding="utf-8", newline="\n").write(r)
        print("edit", f, why)
