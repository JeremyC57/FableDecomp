#!/usr/bin/env python3
"""eetfix.py <file>...: drop a row-local `namespace NEntityEvents { typedef long EEventType; }` stand-in once the row includes
engine/fable_nscript_forward.h (through engine/CScriptThing.h), which forward-declares the PDB enum
`namespace NEntityEvents { enum EEventType; }` (tdfix.py's rule for namespaced typedefs)."""
import sys, re
for f in sys.argv[1:]:
    t = open(f).read()
    if '#include "engine/CScriptThing.h"' not in t: print("skip", f); continue
    t2, k = re.subn(r"^[ \t]*namespace\s+NEntityEvents\s*\{\s*typedef\s+long\s+EEventType\s*;\s*\}[^\n]*\n", "", t, flags=re.M)
    if not k: print("skip", f); continue
    i = t2.find('#include "engine/CScriptThing.h"'); j = t.find("namespace NEntityEvents")
    if j < i: print("skip(order)", f); continue
    open(f, "w", newline="\n").write(t2); print("edit", f)
