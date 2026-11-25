#!/usr/bin/env python3
"""gen432n.py <outdir>: the NPlayerGui draw ctor rows (6) + tests, and the 0x00442760 ClearPrimitives respell, onto the shared headers."""
import sys, os
out = sys.argv[1]
os.makedirs(out + "/rows", exist_ok=True); os.makedirs(out + "/tests", exist_ok=True)
VEC = "AAV?$vector@PAVCDrawBase@NPlayerGui@@V?$allocator@PAVCDrawBase@NPlayerGui@@@std@@@std@@@Z"
C = [  # addr, class, vtable, header, extra defines, size-mode pragmas, extra decls
 ("0064d2a9", "CDrawSpiritsHealth", "0x0125A538", "NPlayerGui_CDrawSpiritsHealth.h", [], ['#pragma optimize("s",on)'], ""),
 ("0064d335", "CDrawReceivedItems", "0x0125A574", "NPlayerGui_CDrawReceivedItems.h", ["FABLE_RETAIL_NPLAYERGUI_CDRAWRECEIVEDITEMS_PASS23"], ['#pragma optimize("s",on)'], "class CEnginePrimitivePrepareInfo;   // named by the PASS23 model's Draw, which the header leaves undeclared\n"),
 ("0064d351", "CDrawFishingGame", "0x0125A580", "NPlayerGui_CDrawFishingGame.h", [], ['#pragma optimize("s",on)'], ""),
 ("0064d389", "CDrawSpiritScores", "0x0125A598", "NPlayerGui_CDrawSpiritScores.h", [], ['#pragma optimize("s",on)'], ""),
 ("0064d3da", "CDrawDamage", "0x0125A5A4", "NPlayerGui_CDrawDamage.h", ["FABLE_RETAIL_NPLAYERGUI_CDRAWDAMAGE_CTOR"], ['#pragma optimize("y", off)', '#pragma optimize("s", on)'], ""),
 ("0064da6e", "CDrawScriptScreenText", "0x0125A5C4", "NPlayerGui_CDrawScriptScreenText.h", [], ['#pragma optimize("s",on)'], ""),
]
for a, c, vt, h, defs, prag, extra in C:
    leaf = "??0%s@NPlayerGui@@QAE@AAVCPlayerGui@@%s" % (c, VEC)
    body_extra = "" if c != "CDrawDamage" else (
        "// The DamageValues list member (+0x28) is constructed after the vptr store: its out-of-line STLport constructor 0x00655B18\n"
        "// (`and [esi],0 ; push 14h ; call alloc ; self-linked sentinel`) takes the default allocator by const reference, so retail\n"
        "// passes the address of the empty temporary homed in the consumed argument slot (`lea eax,[ebp+0Fh]`).\n")
    shape = ("CDrawBase(playerGui, illustrators), this class's vtable, then the DamageValues list" if c == "CDrawDamage"
             else "forwards both references to CDrawBase, then this class's vtable")
    row = "\n".join(prag) + f"""
// NPlayerGui::{c}::{c}(CPlayerGui&, std::vector<CDrawBase*>&) -- {shape}
// Leaf: {leaf}
// Retail: Fable.exe (TLC, VC7.1) @ 0x{a.upper()}; size-mode TU as before (the row's pragmas are HEAD's).
// Names (NF432, two sources each): the constructor is ??0{c}@NPlayerGui@@ in Ego_r.pdb and FableWin.pdb publics, and the table already binds that
//   spelling here (witness 0x0043B570 CPlayerGui::CPlayerGui); the vftable {vt} is retail RTTI .?AV{c}@NPlayerGui@@ (its COL at
//   {vt} - 4) and FableWin.pdb ??_7{c}@NPlayerGui@@6B@. Its two slots are CDrawBase's ClearPrimitivesIfNecessary (0x00644627) and
//   ClearPrimitives (0x00442760) in that order (FableWin vftable of CDrawBase; engine/NPlayerGui_CDrawBase.h VSLOTS).
// Base constructor: ??0CDrawBase@NPlayerGui@@QAE@AAVCPlayerGui@@{VEC} 0x0064CC65 (Ego_r / FableWin; table witness 0x0064E1BE).
// Was a local model (struct CDrawBase / {c} with a placeholder v00) emitting ??_7CVtable_{vt[2:].lower()}@@6B@ / ??_7{c}@@6B@.
{body_extra}#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_CTOR
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_VSLOTS
""" + "".join("#define %s\n" % d for d in defs) + extra + f"""#include "engine/{h}"
namespace NPlayerGui {{
{c}::{c}(CPlayerGui& playerGui, std::vector<CDrawBase*>& illustrators)
    : CDrawBase(playerGui, illustrators)
{{
}}
}}
"""
    open(f"{out}/rows/{a}.cpp", "w").write(row)
    dmg = c == "CDrawDamage"
    test = f"""// Behaviour test for 0x{a.upper()} NPlayerGui::{c}::{c} (NF432): links the row object. CDrawBase's constructor and its two virtuals are
// defined here; the object is built in static storage (no destructor runs). Checks: both references reach CDrawBase's constructor by
// address, the vptr is the class's own (not CDrawBase's), and its slot 0 / slot 1 call ClearPrimitivesIfNecessary / ClearPrimitives.
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_CTOR
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_VSLOTS
""" + "".join("#define %s\n" % d for d in defs) + extra + f"""#include "engine/{h}"
#include <stdio.h>
#include <new>
static const void* g_gui = 0; static const void* g_ill = 0; static int g_slot = 0; static const void* g_baseVptr = 0;
NPlayerGui::CDrawBase::CDrawBase(CPlayerGui& playerGui, std::vector<CDrawBase*>& illustrators)
{{
    g_gui = &playerGui; g_ill = &illustrators; g_baseVptr = *(const void* const*)this;
}}
void NPlayerGui::CDrawBase::ClearPrimitivesIfNecessary() {{ g_slot = 1; }}
void NPlayerGui::CDrawBase::ClearPrimitives() {{ g_slot = 2; }}
""" + ("""// The DamageValues list's own constructor and destructor come from the shared STLport header (pick-any copies in this test object).
""" if dmg else "") + f"""static __declspec(noinline) NPlayerGui::CDrawBase* AsBase(void* p) {{ return (NPlayerGui::CDrawBase*)(NPlayerGui::{c}*)p; }}
static double g_obj[(sizeof(NPlayerGui::{c}) + 7) / 8];
static char g_guiStore[16]; static double g_illStore[4];
int main(void)
{{
    CPlayerGui& gui = *(CPlayerGui*)g_guiStore;
    std::vector<NPlayerGui::CDrawBase*>& ill = *(std::vector<NPlayerGui::CDrawBase*>*)g_illStore;
    NPlayerGui::{c}* p = new (g_obj) NPlayerGui::{c}(gui, ill);
    int ok = ((void*)p == (void*)g_obj) && g_gui == (const void*)g_guiStore && g_ill == (const void*)g_illStore;
    ok = ok && *(const void* const*)g_obj != g_baseVptr;
    NPlayerGui::CDrawBase* b = AsBase(g_obj);
    b->ClearPrimitivesIfNecessary(); ok = ok && g_slot == 1;
    b->ClearPrimitives(); ok = ok && g_slot == 2;
    if (ok) {{ printf("{a}_TEST PASS\\n"); return 0; }}
    printf("{a}_TEST FAIL\\n");
    return 1;
}}
"""
    open(f"{out}/tests/{a}_test.cpp", "w").write(test)
# 0x00442760 respell
row = """// NPlayerGui::CDrawBase::ClearPrimitives() -- does nothing (the base draw element keeps no primitives of its own to clear)
// Leaf: ?ClearPrimitives@CDrawBase@NPlayerGui@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00442760, thiscall, a bare `ret`.
// Name (NF432 from NF430's NAME_FIXES; two sources): retail slot 1 of every NPlayerGui draw vftable that does not override it (0x0125A538 ..
//   [0x00644627, 0x00442760]; RTTI COL present), and FableWin.pdb's vftables of CDrawBase / CDrawSpiritsHealth / CDrawDamage put
//   ?ClearPrimitivesIfNecessary@CDrawBase@NPlayerGui@@UAEXXZ at slot 0 and ?ClearPrimitives@CDrawBase@NPlayerGui@@UAEXXZ at slot 1; CDrawMiniMap
//   overrides slot 1 with ClearPrimitives (landed 0x00644660). FableWin's CDrawBase::ClearPrimitives body is empty (/Od prologue + ret),
//   ClearPrimitivesIfNecessary has the Drawing test and lives at 0x00644627 (already bound to that name).
// Was: ?ClearPrimitivesIfNecessary@CDrawBase@NPlayerGui@@UAEXXZ (an Xbox 360 Test PDB slot order; the PC PDB of the same code disagrees).
#include "engine/NPlayerGui_CDrawBase.h"
namespace NPlayerGui {
void CDrawBase::ClearPrimitives()
{
}
}
"""
open(f"{out}/rows/00442760.cpp", "w").write(row)
test = """// Behaviour test for 0x00442760 NPlayerGui::CDrawBase::ClearPrimitives (NF432): links the row object and calls the member (qualified,
// no vftable) on a filled object image; the body is empty, so the image must be unchanged afterwards.
#include "engine/NPlayerGui_CDrawBase.h"
#include <stdio.h>
#include <string.h>
static unsigned char g_obj[sizeof(NPlayerGui::CDrawBase)];
int main(void)
{
    unsigned char before[sizeof(g_obj)];
    memset(g_obj, 0x5A, sizeof(g_obj)); memcpy(before, g_obj, sizeof(g_obj));
    NPlayerGui::CDrawBase* p = (NPlayerGui::CDrawBase*)g_obj;
    p->NPlayerGui::CDrawBase::ClearPrimitives();
    if (memcmp(before, g_obj, sizeof(g_obj)) == 0) { printf("PASS_00442760\\n"); return 0; }
    printf("FAIL_00442760\\n");
    return 1;
}
"""
open(f"{out}/tests/00442760_test.cpp", "w").write(test)
print("ok")
