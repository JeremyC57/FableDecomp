// NUISystem::CHoverable::`vector deleting destructor'(unsigned int) `adjustor{24}' -- CActionParam-subobject (+0x18) vtable entry:
//   this -= 0x18, jump to the deleting destructor 0x0055C4A0
// Leaf: ??_ECHoverable@NUISystem@@WBI@AEPAXI@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0055C200, 8 bytes: `sub ecx,18h ; jmp 0x0055C4A0`. Compiler-generated adjustor thunk.
// Identity (two sources): slot 0 of CHoverable's CActionParam vtable 0x0124BF88 at +0x18 (its only reference; RTTI .?AVCHoverable@NUISystem@@), and its jump target
//   0x0055C4A0 is slot 0 of the primary vtable 0x0124BFB4, the class's ??_G; VC7.1 spells the +0x18 thunk ??_E...W BI@ (adjustor 24), as
//   objdump -t of this object shows. Twin of the matched +4 thunk 0x0055C1F0 (??_ECHoverable@NUISystem@@W3AEPAXI@Z).
// Was: the placeholder CMulti_0055c4a0::Override_0055c4a0 adjustor (local CFirstBase / CSecondBase / CMulti_0055c4a0 model,
//   rebuild/src/compiled/00/55/unattributed_Override0055c4a0_0055c200.cpp).
// Callee: ??_ECHoverable@NUISystem@@UAEPAXI@Z 0x0055C4A0 (the weak ??_E alias of the ??_G), as 0x0055C1F0 binds it.
// TU: as 0x0055C1F0: the constructor's TU, CHoverable(long) (0x0055BA20; its own row, not graded here). Only the thunk is this row.
// Header: engine/NUISystem_CClickable.h.
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CClickable.h"
namespace NUISystem {
CHoverable::CHoverable(long def)
    : CChangingStateComponent(def)
    , StateBeforeHovered(0)
    , WasHovered(false)
    , PActionsOnHovered(0)
    , PActionsOnUnhovered(0)
{
    CManager::GetInstance()->AddObserver(this);
}
}
