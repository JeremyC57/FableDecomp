#!/usr/bin/env python3
"""Wave 8 family ST3: pimpfix.py (a non-template PImp pointer class -> the CCountedPointer<CScriptThing> template) then
mig_st2.py --implicit on the result; the row is written only when both succeed. usage: mig_st3.py [--dry] <file>..."""
import sys, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import pimpfix, mig_st2, mig_st
mig_st.IMPLICIT = True
if __name__ == "__main__":
    dry = "--dry" in sys.argv
    for f in [a for a in sys.argv[1:] if not a.startswith("--")]:
        t = open(f, encoding="utf-8").read()
        r1, w1 = pimpfix.fix(t)
        if r1 is None: print("skip", f, "pimpfix:", w1); continue
        r2, w2 = mig_st2.fix(r1)
        if r2 is None: print("skip", f, "st2:", w2); continue
        if not dry: open(f, "w", encoding="utf-8", newline="\n").write(r2)
        print("edit", f, w1, "|", w2)
