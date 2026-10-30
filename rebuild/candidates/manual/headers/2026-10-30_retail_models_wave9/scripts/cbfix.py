#!/usr/bin/env python3
"""cbfix.py: a row-local `struct CBase { [CBase();|CBase() {}] virtual ~CBase(); };` used only as a base class is the
retail CBaseClass (RTTI .?AVCScriptThing@@ : CBaseClass; ~CBaseClass 0x0099A2E0 slot 0, CBaseClass() 0x0099A2D0):
the block goes, engine/CBaseClass.h is included in its place, and every `: public CBase` becomes `: public CBaseClass`.
Relocations to ??0CBase / ??1CBase become the bound CBaseClass spellings (mcheck reports them)."""
import re, sys, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from common import find_block, norm
def code_of(t): return re.sub(r"//[^\n]*", "", t)
def fix(t):
    b = find_block(t, "CBase")
    if not b: return None, "no CBase"
    if find_block(t, "CBase", b[1]): return None, "two CBase"
    n = norm(code_of(t[b[0]:b[1]]))
    if not re.fullmatch(r"(?:struct|class) CBase \{ (?:public: )?(?:CBase\(\) ?(?:;|\{ ?\}) )?virtual ~CBase\(\) ?; \};", n): return None, "CBase shape " + n[:60]
    rest = code_of(t[:b[0]] + t[b[1]:])
    uses = len(re.findall(r"\bCBase\b", rest)); bases = len(re.findall(r":\s*public\s+CBase\b", rest))
    if uses != bases: return None, "CBase named elsewhere"
    inc = '#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (the row\'s local CBase)\n'
    if re.search(r'#include "engine/CBaseClass.h"', t[:b[0]]): inc = ""
    out = t[:b[0]] + inc + t[b[1]:]
    out = re.sub(r"(:\s*public\s+)CBase\b", r"\1CBaseClass", out)
    return out, "ok"
