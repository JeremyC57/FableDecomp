#!/usr/bin/env python3
"""Wave 7 item 4 (family ITC): the NScript predicate leaves on their shared headers. Rows whose CScriptThing is already
shared and that define, right after `namespace NScript {`, a local CPredicateBase of the header's shape (operator() /
Clone pure, inline virtual dtor) followed by CIsThingAlive / CIsThingConscious of the declaration shape (ctor from
const CScriptThing&, operator(), Clone, `CScriptThing Thing;`): the blocks go, engine/CIsThingAlive.h /
engine/CIsThingConscious.h (which bring CPredicateBase.h) are included before the namespace.
usage: mig_itc.py [--dry] <file>..."""
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from common import find_block, norm
PB = r"class CPredicateBase ?\{ ?public: ?virtual bool operator ?\( ?\) ?\( ?\) ?const ?= ?0; ?virtual CPredicateBase ?\* ?Clone ?\( ?\) ?const ?= ?0; ?virtual ~CPredicateBase ?\( ?\) ?\{ ?\} ?\};"
def leaf_rx(c):
    its = [r"%s ?\( ?const CScriptThing ?& ?\w* ?\) ?;" % c, r"virtual bool operator ?\( ?\) ?\( ?\) ?const ?;",
           r"virtual CPredicateBase ?\* ?Clone ?\( ?\) ?const ?;", r"CScriptThing Thing ?;"]
    return c, its
def fix(t):
    if not re.search(r'^\s*#include "engine/CScriptThing.h"', t, re.M) or find_block(t, "CScriptThing"): return None, "ST not shared"
    pb = find_block(t, "CPredicateBase")
    if not pb or not re.fullmatch(PB, norm(t[pb[0]:pb[1]])): return None, "pb shape"
    m = re.search(r"namespace NScript\s*\{\s*$", t[:pb[0]])
    if not m: return None, "pb not first in namespace NScript"
    out = t; incs = []
    for c in ("CIsThingAlive", "CIsThingConscious"):
        b = find_block(out, c)
        if not b: continue
        n = norm(out[b[0]:b[1]])
        mm = re.fullmatch(r"class %s ?: ?public CPredicateBase ?\{ ?public: ?(.*)\};" % c, n)
        if not mm: return None, c + " head"
        body = mm.group(1)
        parts = [p.strip() + ";" for p in body.split(";") if p.strip()]
        _, need = leaf_rx(c)
        if len(parts) != len(need) or not all(any(re.fullmatch(r, p) for p in parts) for r in need): return None, c + " shape"
        out = out[:b[0]] + out[b[1]:]; incs.append(c)
    if not incs: return None, "no leaf"
    pb = find_block(out, "CPredicateBase")
    out = out[:pb[0]] + out[pb[1]:]
    m = re.search(r"namespace NScript\s*\{\s*$", out[:pb[0]])
    inc = ("// NScript predicates: the retail models from the shared headers (CPredicateBase vtable slots operator() / Clone /"
           " ~; " + ", ".join(incs) + ": : CPredicateBase, CScriptThing Thing +0x04, 0x10).\n")
    inc += "".join('#include "engine/%s.h"\n' % c for c in incs)
    out = out[:m.start()] + inc + out[m.start():]
    return out, "ok " + ",".join(incs)
if __name__ == "__main__":
    dry = "--dry" in sys.argv
    for f in [a for a in sys.argv[1:] if not a.startswith("--")]:
        t = open(f, encoding="utf-8").read(); r, why = fix(t)
        if r is None: print("skip", f, why); continue
        if not dry: open(f, "w", encoding="utf-8", newline="\n").write(r)
        print("edit", f, why)
