#!/usr/bin/env python3
"""Wave 2 family BC-inline: a row-local CBaseClass with an inline default ctor (`CBaseClass() { }`, optionally a `v00` stand-in
for slot 0, an out-of-line virtual dtor or an empty copy ctor) that the row only uses as `: CBaseClass()` inside a derived
copy ctor becomes the shared engine/CBaseClass.h, and the copy ctor's base initializer becomes the implicit CBaseClass copy
`: CBaseClass(other)` (retail's inline 0x01231710 store immediately overwritten by the derived vtable; LESSONS.md).
A `virtual void v00()` stand-in left on a derived class is dropped when the derived class only had it to mirror the base.
usage: mig_bc_copy.py [--dry] <file>..."""
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from common import find_block, norm
OK = re.compile(r"^(?:class|struct) CBaseClass ?\{ ?public: ?(?:virtual void v0+\(\); ?|CBaseClass\(\) ?\{ ?\} ?|virtual ?~CBaseClass\(\); ?"
                r"|CBaseClass\(const CBaseClass ?& ?\w*\) ?\{ ?\} ?)+\};$")
REPL = '#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0, implicit copy\n'
COPY = re.compile(r"(\(\s*const\s+(\w+)\s*&\s*(\w+)\s*\)\s*:\s*)CBaseClass\(\)")
def fix(t):
    b = find_block(t, "CBaseClass")
    if not b: return None, "noblock"
    if not OK.match(norm(t[b[0]:b[1]])): return None, "shape"
    if "CBaseClass() {" not in t[b[0]:b[1]] and "CBaseClass() { }" not in norm(t[b[0]:b[1]]): return None, "no inline ctor"
    body = t[:b[0]] + REPL + t[b[1]:]
    n = len(COPY.findall(body))
    if not n: return None, "no copy-ctor use"
    body = COPY.sub(lambda m: m.group(1) + "CBaseClass(" + m.group(3) + ")", body)
    rest = re.sub(r"//[^\n]*", "", body)
    if re.search(r"\bCBaseClass\(\)", rest): return None, "other CBaseClass() use"
    return body, f"ok copies={n}"
if __name__ == "__main__":
    dry = "--dry" in sys.argv
    for f in [a for a in sys.argv[1:] if not a.startswith("--")]:
        t = open(f, encoding="utf-8").read()
        r, why = fix(t)
        if r is None: print("skip", f, why); continue
        if not dry: open(f, "w", encoding="utf-8", newline="\n").write(r)
        print("edit", f, why)
