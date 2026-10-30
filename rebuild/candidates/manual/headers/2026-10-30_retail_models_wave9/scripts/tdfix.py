#!/usr/bin/env python3
"""tdfix.py <file>...: a row-local `typedef long|int E...;` stand-in for a PDB enum that engine/fable_nscript_forward.h
forward-declares clashes once a shared NScript header is included. Drop every such typedef and include the forward
header at the first one's place (enum arguments are passed exactly like long; enumfix.py casts int literals)."""
import sys, re, os
R = os.environ["FABLE_ROOT"]
FW = open(R + "/rebuild/include/engine/fable_nscript_forward.h").read()
ENUMS = set(re.findall(r"^enum (\w+);", FW, re.M))
for f in sys.argv[1:]:
    t = open(f).read()
    rx = re.compile(r'^[ \t]*typedef\s+(?:long|int|unsigned long|unsigned int)\s+(\w+)\s*;[^\n]*\n', re.M)
    ms = [m for m in rx.finditer(t) if m.group(1) in ENUMS]
    if not ms: print("skip", f); continue
    inc = '#include "engine/fable_nscript_forward.h"   // the PDB enums the row used typedef stand-ins for\n'
    out = []; last = 0
    for i, m in enumerate(ms):
        out.append(t[last:m.start()]);
        if i == 0 and inc not in t: out.append(inc)
        last = m.end()
    out.append(t[last:]); open(f, "w", newline="\n").write("".join(out))
    print("edit", f, ",".join(m.group(1) for m in ms))
