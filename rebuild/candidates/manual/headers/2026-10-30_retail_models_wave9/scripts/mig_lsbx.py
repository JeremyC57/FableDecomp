#!/usr/bin/env python3
"""Wave 8 family LSBX (wave 7 NEXT item 5): the 5 out-of-line CLevelScriptBase<T> ctor rows 0x00CDD160..0x00CDD220 drop
their local CScriptBase / CLevelScriptBase<TData> and include engine/CLevelScriptBase.h, whose ctor is the inline one
retail expands in every script Alloc; the row's explicit instantiation of that ctor emits the out-of-line copy."""
import sys, re, os
for f in sys.argv[1:]:
    t = open(f).read()
    i = t.index("class CScriptBase {"); j = t.index("namespace NScript {", i)
    inc = ("// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :\n"
           "//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44;\n"
           "//   its ctor is inline (wave 7): this row is the explicit instantiation of it, the five retail copies 0x00CDD160..0x00CDD220).\n"
           "#define FABLE_RETAIL_NSCRIPT\n#include \"engine/CLevelScriptBase.h\"\n")
    t2 = t[:i] + inc + t[j:]
    open(f, "w", newline="\n").write(t2); print("edit", f)
