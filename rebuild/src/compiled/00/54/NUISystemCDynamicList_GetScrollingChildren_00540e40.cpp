// NUISystem::CDynamicList::GetScrollingChildren() -- returns ScrollingChildren
// Leaf: ?GetScrollingChildren@CDynamicList@NUISystem@@UAEAAV?$vector@PAVCComponent@NUISystem@@V?$allocator@PAVCComponent@NUISystem@@@std@@@std@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00540E40, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 149 of NUISystem::CDynamicList's vtable 0x01246E44 (the slot holds this body in 1 vtable(s), owner NUISystem::CDynamicList); the Xbox 360 Test PDB virtual order (third source) aligned on 66 TLC-named slots of that vtable puts GetScrollingChildren(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetScrollingChildren@CDynamicList@NUISystem@@UAEAAV?$vector@PAVCComponent@NUISystem@@V?$allocator@PAVCComponent@NUISystem@@@std@@@std@@XZ; `ret` matches the signature.
// Was: NUISystem::CDynamicList::vfunc_149 (rebuild/src/compiled/00/54/gapscan_sub00540e40_00540e40.cpp).
#define FABLE_RETAIL_CDYNAMICLIST_VSLOTS
#define FABLE_RETAIL_CDYNAMICLIST_MEMBERS
#include "engine/NUISystem_CDynamicList.h"
namespace NUISystem {
std::vector<CComponent*>& CDynamicList::GetScrollingChildren()
{
    return ScrollingChildren;
}
}
