#!/usr/bin/env python3
"""handfix_nf335.py <tests dir> (NF335): the hand edits on top of testmove.py's output, applied the same way every time.
Literal object pointers in four landed tests become real, zero-initialised, long-aligned static storage (the row and the stubs read none
of it): 0x00892F10's two `this` values (RunCase's literal selves) and the `*(CPlayer*)0xAAAA` reference that the GetMainPlayer stub returns
in 0x00899D50 / 0x0089A070 / 0x0089A0E0. Every case and the PASS line are kept. Exits 1 if an anchor is missing."""
import sys, os
d = sys.argv[1]
note = ("// NF335: the landed test used a literal address here; `this` / the player reference now point at real, zero-initialised storage\n"
        "// (the row and the stub read none of it), so no member is called and no reference is bound through an invalid pointer.\n")
def edit(f, pairs):
    p = os.path.join(d, f); t = open(p).read()
    for o, n in pairs:
        if t.count(o) != 1: sys.exit(f"anchor {o!r} in {f}: {t.count(o)}")
        t = t.replace(o, n)
    open(p, "w").write(t); print("fixed", f)
edit("00892f10_test.cpp", [("static int RunCase(", note + "static unsigned long g_firstSelfStorage[4];\nstatic unsigned long g_secondSelfStorage[4];\n\nstatic int RunCase("),
                           ("(const void*)0x11111111,", "(const void*)g_firstSelfStorage,"), ("(const void*)0xeeeeeeee,", "(const void*)g_secondSelfStorage,")])
for f in ("00899d50_test.cpp", "0089a070_test.cpp", "0089a0e0_test.cpp"):
    edit(f, [("CPlayer& CPlayerManager::GetMainPlayer()", note + "static unsigned long g_mainPlayerStorage[4];\nCPlayer& CPlayerManager::GetMainPlayer()"),
             ("return *(CPlayer*)0xAAAA;", "return *(CPlayer*)g_mainPlayerStorage;")])
