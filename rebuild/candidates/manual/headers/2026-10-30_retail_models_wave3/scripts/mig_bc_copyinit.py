#!/usr/bin/env python3
"""Wave 3: a row-local `virtual ~CBaseClass();` model has no user-declared ctor, so a user-written copy ctor of a direct
CBaseClass subclass that omits the base initializer default-constructs the base inline (a vtable store the derived store
kills). On the shared engine/CBaseClass.h the default ctor is the out-of-line 0x0099A2D0, so the same omission becomes a
call. Retail copies the base: give such copy ctors the implicit base copy `CBaseClass(other)` (LESSONS wave 2 item 1).
usage: mig_bc_copyinit.py [--dry] <file>..."""
import sys, re
def fix(t):
    if '#include "engine/CBaseClass.h"' not in t: return None, "no header"
    derived = set(re.findall(r"(?:class|struct)\s+(\w+)\s*:\s*public\s+CBaseClass\b", t))
    n = 0
    for c in derived:
        rx = re.compile(r"(\b" + c + r"\s*\(\s*const\s+" + c + r"\s*&\s*(\w+)\s*\)\s*)(:\s*(?!CBaseClass\()|\{)")
        def rep(m):
            nonlocal n; n += 1
            return m.group(1) + (": CBaseClass(" + m.group(2) + "), " if m.group(3).startswith(":") else ": CBaseClass(" + m.group(2) + ") {")
        t = rx.sub(rep, t)
    return (t, f"ok copies={n}") if n else (None, "nothing")
if __name__ == "__main__":
    dry = "--dry" in sys.argv
    for f in [a for a in sys.argv[1:] if not a.startswith("--")]:
        t = open(f, encoding="utf-8").read(); r, why = fix(t)
        if r is None: print("skip", f, why); continue
        if not dry: open(f, "w", encoding="utf-8", newline="\n").write(r)
        print("edit", f, why)
