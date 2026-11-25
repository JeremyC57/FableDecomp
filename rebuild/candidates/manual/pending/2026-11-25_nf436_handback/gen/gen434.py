#!/usr/bin/env python3
"""gen434.py <batch> <spec.tsv>: NF434 generator for the 0x48 NScript level-script Alloc shape (CLevelScriptBase<T>, no members of its own).
spec columns: addr class T ovr(R,M,I,O letters) slots(retail addresses of own overrides, comma) leafsym oldsrc
Writes batches/<b>/rows/<addr>.cpp, tests/<addr>_test.cpp, hdr/engine/<C>.h (new header) and appends to $W/hdr/engine."""
import sys, os, re
W = "/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF436"
b, spec = sys.argv[1:3]; B = W + "/batches/" + b
for d in ("rows", "tests", "hdr/engine"): os.makedirs(B + "/" + d, exist_ok=True)
os.makedirs(W + "/hdr/engine", exist_ok=True)
NAMES = {"R": "RegisterMain", "M": "Main", "I": "Init", "O": "OnPersist"}
SLOT = {"R": 1, "M": 2, "I": 3, "O": 4}
for line in open(spec).read().splitlines():
    if not line or line.startswith("#"): continue
    a, C, T, ovr, slots, size, vt, alloc = line.split("\t")
    A = a.upper(); slots = slots.split(",")
    G = "FABLE_ENGINE_%s_H" % C.upper()
    decl = []
    for k, ch in enumerate(ovr):
        sig = "CPersistContext&" if ch == "O" else ""
        decl.append("    virtual void %s(%s);%s// slot %d  0x%s" % (NAMES[ch], sig, " " * max(1, 40 - len(NAMES[ch]) - len(sig)), SLOT[ch], slots[k].upper()))
    hdr = f"""#ifndef {G}
#define {G}
// NScript::{C} (NF434, new header; no includer before NF434). The quest-script default shape: CLevelScriptBase<NScript::{T}> with no
// members of its own.
// Layout: Ego_r.pdb {C} (struct_layouts_egor.tsv, {int(size, 16) + 0x14} bytes) at retail size = PDB - 0x14 = 0x{int(size, 16):X} (retail CScriptBase is
// 0x40); the landed Alloc 0x{A} news 0x{int(size, 16):X}.
// Methods (FableWin.pdb LF_CLASS field list; the ctor is user-declared and Ego_r.pdb has no out-of-line copy, so it is inline; the
// destructor is compiler-generated): ctor (CScriptDataBase*, CGameScriptInterfaceBase&), static Alloc, and the public virtual overrides
// below, in the FableWin.pdb ??_7{C}@NScript@@6B@ slot order (retail vftable 0x{vt.upper()}).
// Includers define FABLE_RETAIL_CLEVELSCRIPTBASE as well: the implicit destructor then calls ~CScriptBase 0x00CBD510 directly, as retail.
#ifdef FABLE_RETAIL_NSCRIPT   // the retail NScript model only; no #error branch (VC7.1 under Wine spins on #error)
#include "rebuild_abi.h"
#include "engine/CLevelScriptBase.h"
class CScriptDataBase;
class CGameScriptInterfaceBase;
namespace NScript {{
class {T};
class {C} : public CLevelScriptBase<{T}> {{
public:
    {C}(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<{T}>(data, game) {{ }}
    static CScriptBase* FABLE_FASTCALL Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game); // 0x{A}
{chr(10).join(decl)}
}};
}}
FABLE_STATIC_ASSERT(sizeof(NScript::{C}) == 0x{int(size, 16):X});

#endif // FABLE_RETAIL_NSCRIPT
#endif // {G}
"""
    open(B + "/hdr/engine/%s.h" % C, "w").write(hdr); open(W + "/hdr/engine/%s.h" % C, "w").write(hdr)
    open(B + "/rows/%s.cpp" % a, "w").write(f"""// NScript::{C}::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new {C}(data, game)
// Leaf: {alloc}
// Retail: Fable.exe (TLC, VC7.1) @ 0x{A}, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Callees: scalar operator new 0x00BFEA1A (0x{int(size, 16):X} = sizeof), CScriptBase::CScriptBase 0x00CB8110; the constructor is inlined
//   (CLevelScriptBase<T>(data, game): Game +0x40, MasterScriptData +0x44, then the script vtable 0x{vt.upper()}).
// Model (NF434): the shared engine/{C}.h (new: Ego_r.pdb layout, FableWin.pdb overrides) on engine/CLevelScriptBase.h, replacing a local
//   class that declared none of the class's own virtuals (its vftable bound CScriptBase's slots) and an out-of-line destructor (FableWin.pdb
//   marks it compiler-generated). This TU now also emits the implicit ??_G with the vftable, as retail's TU did (the ??_G follows Alloc).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#include "engine/{C}.h"
CScriptBase* FABLE_FASTCALL NScript::{C}::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{{
    return new {C}(data, game);
}}
""")
    calls = "\n".join("    p->%s(%s); ok = ok && g_last == %d;" % (NAMES[ch], "*(CPersistContext*)g_ctx" if ch == "O" else "", SLOT[ch]) for ch in ovr)
    defs = "\n".join("void Owner::%s(%s) { g_last = %d; }" % (NAMES[ch], "CPersistContext&" if ch == "O" else "", SLOT[ch]) for ch in ovr)
    base = "\n".join("void CScriptBase::%s(%s) { g_last = %d; }" % (NAMES[ch], "CPersistContext&" if ch == "O" else "", 10 + SLOT[ch]) for ch in "RMIO")
    open(B + "/tests/%s_test.cpp" % a, "w").write(f"""// Behaviour test for 0x{A} NScript::{C}::Alloc (NF434): links the row object and calls Alloc. The out-of-line callees the row object
// references are defined here and record their calls: CScriptBase's constructor / destructor as __fastcall stand-ins bound by
// /alternatename (the 0x008C01B0 test pattern), operator new / delete, and the virtuals of both vftables the object emits. Checks: one
// 0x{int(size, 16):X}-byte allocation, CScriptBase() on it, Game +0x40 / MasterScriptData +0x44 stored, the class's own overrides reached through the
// vptr (not CScriptBase's), and `delete` reaching ~CScriptBase on this and freeing it (the implicit ??_G).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#include "engine/{C}.h"
#include <stdio.h>
#include <stdlib.h>
typedef NScript::{C} Owner;
static int g_last = 0; static int g_ctor = 0; static void* g_ctorThis = 0; static void* g_dtorThis = 0;
static unsigned g_size = 0; static void* g_new = 0; static void* g_freed = 0; static long g_ctx[4];
{defs}
{base}
extern "C" void* __fastcall nf434_base_ctor(void* self) {{ ++g_ctor; g_ctorThis = self; return self; }}
extern "C" void* __fastcall nf434_base_dtor(void* self) {{ g_dtorThis = self; return self; }}
#pragma comment(linker, "/alternatename:??0CScriptBase@@QAE@XZ=@nf434_base_ctor@4")
#pragma comment(linker, "/alternatename:??1CScriptBase@@UAE@XZ=@nf434_base_dtor@4")
void* operator new(size_t n) {{ g_size = (unsigned)n; g_new = malloc(n); return g_new; }}
void operator delete(void* p) {{ g_freed = p; free(p); }}
int main(void)
{{
    static long data[4]; static long game[4];
    CScriptBase* b = Owner::Alloc((CScriptDataBase*)data, *(CGameScriptInterfaceBase*)game);
    Owner* p = (Owner*)b;
    int ok = b != 0 && (void*)b == g_new && g_size == 0x{int(size, 16):X} && g_ctor == 1 && g_ctorThis == (void*)b;
    ok = ok && (void*)p->Game == (void*)game && (void*)p->MasterScriptData == (void*)data;
{calls}
    delete p;
    ok = ok && g_dtorThis == (void*)b && g_freed == (void*)b;
    if (ok) {{ printf("{a}_TEST PASS\\n"); return 0; }}
    printf("{a}_TEST FAIL\\n");
    return 1;
}}
""")
    print("gen", a, C)
