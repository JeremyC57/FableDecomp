#!/usr/bin/env python3
"""Wave 2 family PDCB: a row-local opaque CParentDefClassBase stand-in (any mix of: virtual or plain ~CParentDefClassBase(),
`virtual void v00()/v000()`, opaque byte/dword arrays, the default ctor, copy ctor and operator= declarations) becomes the
shared retail engine/CParentDefClassBase.h (FABLE_RETAIL_DEF_CLASS_BASE; vtable 0x01230CBC, 0x39 bytes packed; ctor
0x00430370, ~ 0x00430300, copy 0x0046EE98, operator= 0x00431ED0). Rows that also define a def-chain class locally are left
alone (other families). Idempotent. usage: mig_pdcb.py [--dry] <file>..."""
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from common import find_block, norm
ITEM = re.compile(r"(?:virtual ?~CParentDefClassBase\(\) ?; ?|~CParentDefClassBase\(\) ?; ?|virtual void v0+\(\) ?; ?"
                  r"|(?:unsigned char|unsigned long|long|char|int|void\*) ?\w+ ?\[[0-9A-Fa-fx +\-*]+\] ?; ?"
                  r"|CParentDefClassBase\(\) ?; ?|CParentDefClassBase\(const CParentDefClassBase ?& ?\w*\) ?; ?"
                  r"|CParentDefClassBase ?& ?operator ?= ?\(const CParentDefClassBase ?& ?\w*\) ?; ?)")
HEAD = re.compile(r"^(?:class|struct) CParentDefClassBase ?\{ ?public: ?(.*)\};$")
CHAIN = ("CIVCountedPointeeBase", "CResource", "CDefPointeeBase", "CDefClassBase", "CSubDefClassBase")
REPL = ("// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:\n"
        "//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,\n"
        "//   operator= 0x00431ED0).\n#define FABLE_RETAIL_DEF_CLASS_BASE\n#include \"engine/CParentDefClassBase.h\"\n")
def fix(t):
    b = find_block(t, "CParentDefClassBase")
    if not b: return None, "noblock"
    n = norm(t[b[0]:b[1]])
    m = HEAD.match(n)
    if not m: return None, "shape(head): " + n[:160]
    body = m.group(1)
    rest = ITEM.sub("", body).strip()
    if rest: return None, "shape(item): " + rest[:160]
    for c in CHAIN:
        if find_block(t, c): return None, "local " + c
    if "engine/CParentDefClassBase.h" in t: return None, "already includes"
    return t[:b[0]] + REPL + t[b[1]:], "ok"
if __name__ == "__main__":
    dry = "--dry" in sys.argv
    for f in [a for a in sys.argv[1:] if not a.startswith("--")]:
        t = open(f, encoding="utf-8").read()
        r, why = fix(t)
        if r is None: print("skip", f, why); continue
        if not dry: open(f, "w", encoding="utf-8", newline="\n").write(r)
        print("edit", f, why)
