#!/usr/bin/env python3
"""gro_cp.py [--dry] <file>...: give a GRO row the CCountedPointer class template the STB / MovieBase headers' PImp needs
(retail: CCountedPointer<CScriptGameResourceObjectScriptedThingBase>, Ego_r.pdb), before mig_gro.py runs.
  * a row-local two-word `struct CCountedPointerWords { void* Data; void* Info; ... };` used only as a PImp member is
    respelled `template <class T> class CCountedPointer { public: ... };` (ctor renamed; same members, same inline bodies);
  * a row with no pointer model at all gets the minimal two-word template (zeroing default ctor, no dtor: the rows that
    destroy a base call the out-of-line dtor) in front of its first GRO class.
Rows that already name CCountedPointer are left to cpfix.py. mcheck is the gate."""
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from common import find_block
MIN = ("template <class T> class CCountedPointer { public:      // retail CCountedPointer<T>: Data +0, Info +4 (row model: the\n"
       "    void* Data;                                          // two words only; refcounting is out of line in this row)\n"
       "    void* Info;\n    CCountedPointer() : Data(0), Info(0) { }\n};\n")
GRO = re.compile(r"^[ \t]*(?:class|struct)\s+CScriptGameResourceObject(?:ScriptedThing|Movie)?Base\b[^;]*\{", re.M)
def fix(t):
    if re.search(r"\bCCountedPointer\b", t): return None, "names CCountedPointer"
    b = find_block(t, "CCountedPointerWords")
    if b:
        uses = re.findall(r"\bCCountedPointerWords\b", t[:b[0]] + t[b[1]:])
        pims = re.findall(r"\bCCountedPointerWords\s+PImp\s*;", t)
        if len(uses) != len(pims): return None, "CCountedPointerWords used outside PImp"
        blk = t[b[0]:b[1]]
        blk = re.sub(r"^([ \t]*)struct\s+CCountedPointerWords\s*\{", r"\1template <class T> class CCountedPointer { public:", blk, count=1)
        blk = re.sub(r"\bCCountedPointerWords\b", "CCountedPointer", blk)
        out = t[:b[0]] + blk + t[b[1]:]
        out = re.sub(r"\bCCountedPointerWords(\s+PImp\s*;)", r"CCountedPointer<CScriptGameResourceObjectScriptedThingBase>\1", out)
        return out, "words"
    if re.search(r"\bPImp\b", t): return None, "other PImp model"
    m = GRO.search(t)
    if not m: return None, "no GRO class"
    return t[:m.start()] + MIN + t[m.start():], "minimal"
if __name__ == "__main__":
    for f in [a for a in sys.argv[1:] if not a.startswith("--")]:
        t = open(f, encoding="utf-8").read(); r, why = fix(t)
        if r is None: print("skip", f, why); continue
        if "--dry" not in sys.argv: open(f, "w", encoding="utf-8", newline="\n").write(r)
        print("edit", f, why)
