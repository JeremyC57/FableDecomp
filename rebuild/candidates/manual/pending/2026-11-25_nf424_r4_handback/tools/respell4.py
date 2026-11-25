#!/usr/bin/env python3
"""respell4.py: NF424 round 4 respell of the 85 AES pairs (R-NF424-5 ruling): r3/final/<a>.cpp -> r4/final/<a>.cpp (+ the owner copy
in r4/ov/src/compiled/<path>). Device /d2inlS20 (no /Og) + FABLE_RETAIL_CSPAWNEDFUNCBASE_DTOR_DECL_ONLY; owners get the
`// Flags:` + `// Inline-budget:` lines right after their Leaf line and the six NF424 r3 lines cut to two."""
import os, re, shutil
A = "/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF424/"
FL = "/d2inlS20"
IB = ("// Inline-budget: /d2inlS20 + FABLE_RETAIL_CSPAWNEDFUNCBASE_DTOR_DECL_ONLY -- implicit ~CActiveEntityScriptBase (~CScriptThing +0x48, "
      "~CCountedPointer +0x34, call ~CSpawnedFuncBase) expanded in this TU's ??_G as retail does; sweep on 0x00D032C0: /d2inlS17-19 fail, "
      "20-40 match (with or without /Og; without DECL_ONLY 30v48), 85/120 CActiveEntityScript<T> pairs match at /d2inlS20")
NOTE = ["// NF424 r4 (2026-11-25, R-NF424-5: 2026-11-08 owner rule, inline-budget residue): DECL_ONLY is the existing engine/CSpawnedFuncBase.h",
        "//   opt-in (~CSpawnedFuncBase 0x00CDD4C0 declared only); no header changes. The flag emulates an unidentified build setting of the retail script TUs."]
n = 0
for l in open(A + "r3/work/aes_cand.tsv"):
    a, o, src = l.rstrip("\n").split("\t")
    # thin row
    t = open(A + "r3/final/%s.cpp" % a).read()
    assert t.count("// Flags: /Og /d2inlS30\n") == 1
    t = t.replace("// Flags: /Og /d2inlS30\n", "// Flags: %s\n" % FL).replace("The owner carries the /Og /d2inlS30 + DTOR_DECL_ONLY edit", "The owner carries the /d2inlS20 + DTOR_DECL_ONLY edit")
    assert "/Og" not in t and "d2inlS30" not in t, a
    open(A + "r4/final/%s.cpp" % a, "w").write(t)
    for x in ("_test.cpp",):
        if os.path.exists(A + "r3/final/%s%s" % (a, x)): shutil.copy(A + "r3/final/%s%s" % (a, x), A + "r4/final/%s%s" % (a, x))
    # owner
    L = open(A + "r3/final/%s.cpp" % o).read().split("\n")
    i = next(k for k, s in enumerate(L) if s.startswith("// NF424 r3"))
    assert all(L[k].startswith("//   ") for k in range(i + 1, i + 6)) and L[i + 6] == "// Flags: /Og /d2inlS30", o
    del L[i:i + 7]
    j = next(k for k, s in enumerate(L) if s.startswith("// Leaf:"))
    L[j + 1:j + 1] = ["// Flags: " + FL, IB] + NOTE
    s = "\n".join(L)
    assert s.count("// Flags:") == 1 and "// Flags: /Og" not in s and "d2inlS30" not in s and "NF424 r3" not in s, o
    assert "#define FABLE_RETAIL_CSPAWNEDFUNCBASE_DTOR_DECL_ONLY\n#include \"fable_compiler.h\"" in s or "#define FABLE_RETAIL_CSPAWNEDFUNCBASE_DTOR_DECL_ONLY" in s, o
    open(A + "r4/final/%s.cpp" % o, "w").write(s)
    if os.path.exists(A + "r3/final/%s_test.cpp" % o): shutil.copy(A + "r3/final/%s_test.cpp" % o, A + "r4/final/%s_test.cpp" % o)
    ov = A + "r4/ov/src/compiled/" + src
    os.makedirs(os.path.dirname(ov), exist_ok=True); open(ov, "w").write(s)
    n += 1
print("pairs", n)
