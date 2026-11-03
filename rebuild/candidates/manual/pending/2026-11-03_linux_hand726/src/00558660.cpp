// NUISystem::CKeyRedefiner::~CKeyRedefiner() -- clears the selected-redefiner global, destroys the alias list, then the CClickable base
// Leaf: ??1CKeyRedefiner@NUISystem@@UAE@XZ
// 
// Retail 0x00558660, 60 bytes (catalog label "CScrollingViewport::~CScrollingViewport" was wrong: the vtables it
// stores are CKeyRedefiner's 0x0124ADBC / +4 0x0124AD98 / +0x18 0x0124AD90). Body: if g_pSelectedKeyRedefiner
// (0x013B8AC8) is this, clear it; the compiler then destroys the member `std::list<EGameAction> Actions` (+0x19C,
// ??1?$_List_base@W4EGameAction@@...@std@@QAE@XZ 0x00558AA0) and tail-jumps to ~CClickable 0x0055B760.
// Shape: three vtable stores, compare-and-clear of the global, member list dtor, base dtor jump.
// Header: engine/NUISystem_CKeyRedefiner.h (new; PDB members PActionText +0x194, PKeyText +0x198, Actions +0x19C,
// SubType +0x1A0, Definition +0x1A4; retail size 0x1C0).
#include "engine/NUISystem_CKeyRedefiner.h"
NUISystem::CKeyRedefiner::~CKeyRedefiner()
{
    if (g_pSelectedKeyRedefiner == this)
        g_pSelectedKeyRedefiner = 0;
}
