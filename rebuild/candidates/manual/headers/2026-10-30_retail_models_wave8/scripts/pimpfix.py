#!/usr/bin/env python3
"""pimpfix.py [--dry] <file>...: generalises wave 6 cpfix.py. A row-local NON-template pointer class X (`CCountedPointerWords`,
`CScriptThingPImp`, `CCountedScriptImp`, `CCountedPointer_CThing`, `CCountedPointer` ...) used only as the CScriptThing member
`X PImp;` is the retail CCountedPointer<CScriptThing> (Ego_r.pdb CScriptThing::PImp): X becomes `template <class T> class
CCountedPointer` (its members and inline bodies unchanged, X's self-references respelled) and the member
`CCountedPointer<CScriptThing> PImp;`. Skips a row that already has a CCountedPointer, or names X anywhere else, or whose
X has an out-of-line member (its symbol would change)."""
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from common import find_block
def code_of(t): return re.sub(r"//[^\n]*", "", t)
def fix(t):
    sb = find_block(t, "CScriptThing")
    if not sb: return None, "no CScriptThing"
    m = re.search(r"\b(\w+)\s+PImp\s*;", code_of(t[sb[0]:sb[1]]))
    if not m: return None, "no plain PImp"
    X = m.group(1)
    if X in ("CScriptThing", "void", "long", "int"): return None, "PImp type " + X
    xb = find_block(t, X)
    if not xb or xb[0] > sb[0]: return None, "X undefined before"
    blk = t[xb[0]:xb[1]]
    if re.match(r"\s*template", blk): return None, "X is a template"
    if X != "CCountedPointer" and re.search(r"\bCCountedPointer\b", code_of(t)): return None, "row has a CCountedPointer"
    rest = code_of(t[:xb[0]] + t[xb[1]:])
    uses = re.findall(r"\b" + X + r"\b", rest)
    if len(uses) != len(re.findall(r"\b" + X + r"\s+PImp\s*;", rest)): return None, "X named elsewhere"
    # out-of-line members: a declaration without a body (other than data) would be a call with X in its symbol
    body = code_of(blk[blk.index("{") + 1:blk.rindex("}")])
    flat = re.sub(r"\{[^{}]*\}", "{}", re.sub(r"\{[^{}]*\}", "{}", re.sub(r"\{[^{}]*\}", "{}", body)))
    if re.search(r"\)\s*(?:const\s*)?;", flat): return None, "X has an out-of-line member"
    hm = re.match(r"(\s*)(?:struct|class)\s+" + X + r"\s*\{", blk)
    if not hm: return None, "X head (base class?)"
    pub = "" if re.match(r"\s*class", blk) else " public:"
    newblk = hm.group(1) + "template <class T> class CCountedPointer {" + pub + blk[hm.end():]
    newblk = re.sub(r"\b" + X + r"\b", "CCountedPointer", newblk)
    t2 = t[:xb[0]] + newblk + t[xb[1]:]
    t2 = re.sub(r"\b" + X + r"(\s+PImp\s*;)", r"CCountedPointer<CScriptThing>\1", t2)
    return t2, "ok " + X
if __name__ == "__main__":
    dry = "--dry" in sys.argv
    for f in [a for a in sys.argv[1:] if not a.startswith("--")]:
        t = open(f, encoding="utf-8").read(); r, why = fix(t)
        if r is None: print("skip", f, why); continue
        if not dry: open(f, "w", encoding="utf-8", newline="\n").write(r)
        print("edit", f, why)
