// NUISystem::CIconText::`vector deleting destructor'(unsigned int) `adjustor{24}' -- CActionParam-subobject (+0x18) vtable entry: this -= 0x18,
//   jump to the deleting destructor 0x00543C70
// Leaf: ??_ECIconText@NUISystem@@WBI@AEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00543B40, 8 bytes: `sub ecx,18h ; jmp 0x00543C70`. Compiler-generated adjustor thunk.
// Identity (two sources): slot 0 of CIconText's CActionParam vtable 0x012470A0 at +0x18 (RTTI .?AVCIconText@NUISystem@@; engine/
//   NUISystem_CIconText.h), and its jump target 0x00543C70 is slot 0 of the primary vtable 0x012470CC, the class's ??_G (calls ~CIconText
//   0x005424B0); VC7.1 spells the +0x18 thunk ??_E...W BI@ (adjustor 24), as objdump -t of this object shows. Twin of the matched +4
//   thunk 0x00543B30 (??_ECIconText@NUISystem@@W3AEPAXI@Z).
// Was: the placeholder CMulti_00543c70::Override_00543c70 adjustor (local CFirstBase / CSecondBase / CMulti_00543c70 model,
//   rebuild/src/compiled/00/54/unattributed_Override00543c70_00543b40.cpp).
// Callee: ??_ECIconText@NUISystem@@UAEPAXI@Z 0x00543C70 (the weak ??_E alias of ??_GCIconText@NUISystem@@UAEPAXI@Z).
// TU: as 0x00543C70: the vtables, the ??_G and both adjustor thunks are emitted where the vtable is first needed; the definer is the
//   constructor CIconText(long) (landed row 0x00542330, its body as there) on the shared engine/NUISystem_CIconText.h. The constructor is
//   not graded here (it is its own row); only the thunk is this row.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CICONTEXT_VSLOTS
#include "engine/NUISystem_CIconText.h"
namespace NUISystem {
CIconText::CIconText(long parent) : CChangingStateComponent(parent)
{
    NoRenderNextFrame = 0;
    GetDataFromDef();
    Unavailable = false;
}
}
