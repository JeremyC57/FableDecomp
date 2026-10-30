#!/usr/bin/env python3
"""Wave 4 family BC-v00: a row-local CBaseClass whose block carries an invented slot-0 stand-in (`virtual void v00();`,
`v000`, `v0`, `b00`, `b0`, `VTableAnchor`) in any mix with the trivial members mig_bc.py accepts (`CBaseClass();`,
`CBaseClass() { }`, `__forceinline` / `__declspec(noinline)` forms, `(virtual) ~CBaseClass();` / `{ }`, an empty copy ctor)
becomes the shared engine/CBaseClass.h (vtable 0x01231710: slot 0 is the dtor, 0x0099A2E0; CBaseClass() 0x0099A2D0). Slot 0
of the retail CBaseClass vtable is the (scalar deleting) dtor, so the stand-in IS the dtor slot: a row that calls the
stand-in by name is skipped (it must be rewritten as a delete / dtor call by hand). A derived class that mirrors the stand-in
(`virtual void v00();` inside `class X : public CBaseClass {`) loses that line (it was the slot-0 override). A `: CBaseClass()`
in a copy ctor of a direct subclass becomes `: CBaseClass(other)` when the local ctor was inline (retail's killed vtable
store; wave 2 LESSONS item 1). Run mig_bc_copyinit.py afterwards. Idempotent.
usage: mig_bc_v.py [--dry] [--force-other] <file>..."""
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from common import find_block, norm
STAND = r"(?:v0+|b0+|VTableAnchor)"
ITEM = (r"(?:virtual void " + STAND + r"\(\); ?|(?:__forceinline |__declspec\(noinline\) ?)?CBaseClass\(\) ?(?:\{ ?\} ?|; ?)"
        r"|(?:__declspec\(noinline\) ?)?(?:virtual ?)?~CBaseClass\(\) ?(?:\{ ?\} ?|; ?)|CBaseClass\(const CBaseClass ?& ?\w*\) ?\{ ?\} ?;? ?)")
OK = re.compile(r"^(?:class|struct) CBaseClass ?\{ ?(?:public: ?)?(?:" + ITEM + r")*\};$")
REPL = '#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy\n'
COPY = re.compile(r"(\(\s*const\s+(\w+)\s*&\s*(\w+)\s*\)\s*:\s*)CBaseClass\(\)")
def fix(t):
    if '#include "engine/CBaseClass.h"' in t: return None, "already includes"
    b = find_block(t, "CBaseClass")
    if not b: return None, "noblock"
    blk = norm(t[b[0]:b[1]])
    if not OK.match(blk): return None, "shape"
    names = set(re.findall(r"virtual void (" + STAND + r")\(\)", blk))
    rest = t[:b[0]] + t[b[1]:]
    code = re.sub(r"//[^\n]*", "", rest)
    for n in names:
        if re.search(r"(?:->|\.|::)\s*" + n + r"\s*\(", code): return None, f"calls stand-in {n}"
    inline_ctor = re.search(r"CBaseClass\(\) ?\{", blk) is not None
    out = t[:b[0]] + REPL + t[b[1]:]
    mirrors = 0
    # every class that derives (transitively) from CBaseClass: a stand-in of the same name there was the slot-0 override
    edges = re.findall(r"(?:class|struct)\s+(\w+)\s*:\s*((?:public\s+[\w:<>]+\s*,?\s*)+)\{", out)
    derived = {"CBaseClass"}; ch = True
    while ch:
        ch = False
        for d, bases in edges:
            if d not in derived and any(x in derived for x in re.findall(r"public\s+([\w:]+)", bases)): derived.add(d); ch = True
    derived.discard("CBaseClass")
    for d in sorted(derived):
        db = find_block(out, d)
        if not db: continue
        seg = out[db[0]:db[1]]
        def depth_at(s, i):
            d = 0
            for c in s[:i]:
                if c == "{": d += 1
                elif c == "}": d -= 1
            return d
        for n in names:
            # only the class's own members (brace depth 1), never a nested class's
            for rx in (r"[ \t]*virtual\s+void\s+" + n + r"\s*\(\s*\)\s*;[ \t]*(//[^\n]*)?\n", r"virtual\s+void\s+" + n + r"\s*\(\s*\)\s*;\s*"):
                hits = [m for m in re.finditer(rx, seg) if depth_at(seg, m.start()) == 1]
                for m in reversed(hits): seg = seg[:m.start()] + seg[m.end():]
                mirrors += len(hits)
                if hits: break
        out = out[:db[0]] + seg + out[db[1]:]
    copies = 0
    if inline_ctor:
        def cp(m):
            ctor = re.search(r"(\w+)\s*$", out[:m.start()])
            if ctor and ctor.group(1) != m.group(2): return m.group(0)     # a converting ctor, not the copy ctor
            return m.group(1) + "CBaseClass(" + m.group(3) + ")"
        out, copies = COPY.subn(cp, out)
    if re.search(r"\bCBaseClass\(\)", re.sub(r"//[^\n]*", "", out)) and inline_ctor and not FORCE_OTHER: return None, "other CBaseClass() use with an inline local ctor"
    return out, f"ok stand={','.join(sorted(names)) or '-'} mirrors={mirrors} copies={copies}"
FORCE_OTHER = "--force-other" in sys.argv   # wave 4: let mcheck decide whether the other `CBaseClass()` use reaches the leaf bytes
if __name__ == "__main__":
    dry = "--dry" in sys.argv
    for f in [a for a in sys.argv[1:] if not a.startswith("--")]:
        t = open(f, encoding="utf-8").read()
        r, why = fix(t)
        if r is None: print("skip", f, why); continue
        if not dry: open(f, "w", encoding="utf-8", newline="\n").write(r)
        print("edit", f, why)
