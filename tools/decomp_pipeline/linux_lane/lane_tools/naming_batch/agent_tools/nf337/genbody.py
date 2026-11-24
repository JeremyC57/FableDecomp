#!/usr/bin/env python3
"""genbody.py (NF337) <plan.tsv>: the body-form Copy rows (retail: `call 0x00431F10` CDefClassBase::operator=, then one member copy; the
derived operator= is inline in retail). plan columns: addr, class, member, offset(hex), ctype, defines (comma), header route note, why the explicit form.
Writes work/gen/<a>.cpp (the PDB member on the shared header, explicit form) and work/gen/<a>_test.cpp (NF335 / 0x004644FA moved-test form)."""
import os, sys
S = os.environ["S"]; A = f"{S}/w22/agentNF337"; G = f"{A}/work/gen"
for l in open(sys.argv[1]).read().splitlines():
    if not l.strip() or l.startswith("#"): continue
    a, C, m, off, ct, defs, note, why = l.split("\t")
    U = a.upper(); o = int(off, 16)
    mov = "byte" if ct == "bool" else "dword"
    src = [f"// Leaf: ?Copy@{C}@@UAEXPBVCDefClassBase@@@Z",
           f"// {C}::Copy(const CDefClassBase* other) -- copy another {C}: the CDefClassBase part through its out-of-line operator=, then {m}",
           f"// Retail: Fable.exe (TLC, VC7.1) @ 0x{U}, thiscall, `ret 4`: `call 0x00431F10` (CDefClassBase::operator=, this = ecx, other pushed), then the",
           f"//   {mov} {m} +0x{o:X} from other to this (the derived assignment is inline in retail: no out-of-line {C}::operator= is called).",
           f"// Identity: the RTTI vftable placement of 0x{U} (ghidra_out/labels_rtti_port.tsv, slot 19 Copy) + Ego_r.pdb / FableWin.pdb name sets.",
           f"// Callee: CDefClassBase::operator=(const CDefClassBase&) 0x00431F10, ??4CDefClassBase@@QAEAAV0@ABV0@@Z (Ego_r.pdb + FableWin.pdb; engine/CDefClassBase.h).",
           f"// NF337 (2026-11-24): moved from the flattened __fastcall stand-in onto the PDB member on the shared class model ({note}); the explicit",
           f"//   base call + member copy is the inline assignment's expansion: {why}.",
           "#define FABLE_RETAIL_DEF_CLASS_BASE"] + [f"#define {d}" for d in defs.split(",") if d] + \
          ['#include "rebuild_abi.h"', f'#include "engine/{C}.h"', "",
           f"void {C}::Copy(const CDefClassBase* other)", "{",
           "    CDefClassBase::operator=(*other);",
           f"    {m} = ((const {C}*)other)->{m};", "}", ""]
    open(f"{G}/{a}.cpp", "w").write("\n".join(src))
    if ct == "bool": setv, chk, vt = f"*(unsigned char*)((char*)g_src + 0x{o:x}) = 0x5A;", f"*(unsigned char*)((char*)g_dst + 0x{o:x}) == 0x5A", "unsigned char"
    else: setv, chk, vt = f"*(unsigned long*)((char*)g_src + 0x{o:x}) = 0x7A6B5C4DUL;", f"*(unsigned long*)((char*)g_dst + 0x{o:x}) == 0x7A6B5C4DUL", "unsigned long"
    t = [f"// behaviour test for 0x{U}: {C}::Copy calls CDefClassBase::operator= with this and the source object, then copies {m} (+0x{o:X})",
         f"// NF337 (NF324 Q2 form): the row now defines the PDB member {C}::Copy (?Copy@{C}@@UAEXPBVCDefClassBase@@@Z).",
         "// The test calls it qualified through a pointer to this declaration model, cast from static storage: the model is never",
         "// constructed (no vftable, no RTTI). The callee CDefClassBase::operator= (??4CDefClassBase@@QAEAAV0@ABV0@@Z) is stubbed as a member here.",
         "#include <cstdio>", "",
         "class CDefClassBase { public:", "    CDefClassBase& operator=(const CDefClassBase& other);", "};",
         f"class {C} {{ public:", "    virtual void Copy(const CDefClassBase* other);", "};", "",
         "static int g_calls = 0;", "static const void* g_this = 0;", "static const void* g_other = 0;",
         "CDefClassBase& CDefClassBase::operator=(const CDefClassBase& other)", "{",
         "    ++g_calls; g_this = this; g_other = &other;", "    return *this;", "}", "",
         "static unsigned long g_dst[12];", "static unsigned long g_src[12];", "",
         "int main()", "{", f"    {setv}",
         f"    (({C}*)(g_dst))->{C}::Copy((const CDefClassBase*)(g_src));",
         f"    if (g_calls == 1 && g_this == (const void*)g_dst && g_other == (const void*)g_src && {chk}) {{",
         f'        std::printf("{a}_TEST PASS\\n");', "        return 0;", "    }",
         f'    std::printf("FAIL_{a} calls=%d\\n", g_calls);', "    return 1;", "}", ""]
    open(f"{G}/{a}_test.cpp", "w").write("\n".join(t))
    print("GEN", a, C)
