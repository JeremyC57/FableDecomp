#!/usr/bin/env python3
"""cp_post.py <file>...: after mig_cp, a row-local deleter declaration (`void __fastcall Destroy_<addr>(void*);`) that the
row no longer names is the header's CCountedPointer<T>::DeleteData: drop the declaration, note it in a comment."""
import sys, re
for f in sys.argv[1:]:
    t = open(f, encoding="utf-8").read()
    m = re.search(r"(?m)^void\s+__fastcall\s+(Destroy_[0-9a-f]{8})\s*\(\s*void\s*\*\s*\w*\s*\)\s*;[^\n]*\n", t)
    if not m: continue
    rest = t[:m.start()] + t[m.end():]
    if re.search(r"\b%s\b" % m.group(1), re.sub(r"//[^\n]*", "", rest)): print("skip", f, "used"); continue
    a = m.group(1)[-8:].upper()
    t = t[:m.start()] + "// 0x%s is CCountedPointer<T>::DeleteData (engine/CCountedPointer.h; reloc_bindings binds both spellings)\n" % a + t[m.end():]
    open(f, "w", encoding="utf-8", newline="\n").write(t); print("edit", f)
