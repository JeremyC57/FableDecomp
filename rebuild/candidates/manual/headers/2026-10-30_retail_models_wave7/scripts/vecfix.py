#!/usr/bin/env python3
"""vecfix.py [--dry] <file>...: a row-local global `template <class T> struct|class vector { ... };` (the 12-byte STLport
model) moves into namespace std as `template <class T, class A = allocator<T> > class vector` (public members unchanged),
followed by `using std::vector;`, so it is the std::vector<T, std::allocator<T> > the CGameScriptInterface header slots
take. Bytes: same members, same inline bodies. Skips a row whose vector is already in a namespace."""
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from common import find_block
def fix(t):
    m = re.search(r"(?m)^template\s*<\s*(?:class|typename)\s+(\w+)\s*>\s*(?:struct|class)\s+vector\b\s*(:[^{]*)?\{", t)
    if not m: return None, "no global vector template"
    pre = t[:m.start()]
    if pre.count("{") != pre.count("}"): return None, "vector inside a scope"
    b = find_block(t, "vector", m.start())
    if not b or b[0] != m.start(): return None, "block"
    T = m.group(1); base = m.group(2) or ""
    head = f"namespace std {{\ntemplate <class {T}> class allocator;\ntemplate <class {T}, class A = allocator<{T}> > class vector {base}{{\npublic:"
    blk = t[b[0]:b[1]]
    body = blk[m.end() - m.start():]
    new = head + body.rstrip("\n") + "\n}\nusing std::vector;\n"
    return t[:b[0]] + new + t[b[1]:], "ok"
if __name__ == "__main__":
    dry = "--dry" in sys.argv
    for f in [a for a in sys.argv[1:] if not a.startswith("--")]:
        t = open(f, encoding="utf-8").read(); r, why = fix(t)
        if r is None: print("skip", f, why); continue
        if not dry: open(f, "w", encoding="utf-8", newline="\n").write(r)
        print("edit", f, why)
