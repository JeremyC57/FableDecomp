// NUISystem::CDynamicList::GetSelectedIndex() -- returns SelectionIndex
// Leaf: ?GetSelectedIndex@CDynamicList@NUISystem@@UAEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00540E30, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 143 of NUISystem::CDynamicList's vtable 0x01246E44 (the slot holds this body in 1 vtable(s), owner NUISystem::CDynamicList); the Xbox 360 Test PDB virtual order (third source) aligned on 66 TLC-named slots of that vtable puts GetSelectedIndex(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetSelectedIndex@CDynamicList@NUISystem@@UAEKXZ; `ret` matches the signature.
// Was: NUISystem::CDynamicList::vfunc_143 (rebuild/src/compiled/00/54/gapscan_sub00540e30_00540e30.cpp).
#define FABLE_RETAIL_CDYNAMICLIST_VSLOTS
#define FABLE_RETAIL_CDYNAMICLIST_MEMBERS
#include "engine/NUISystem_CDynamicList.h"
namespace NUISystem {
unsigned long CDynamicList::GetSelectedIndex()
{
    return SelectionIndex;
}
}
