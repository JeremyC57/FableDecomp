// std::_Rb_tree<CThingCreatureBase*, std::pair<CThingCreatureBase* const, CAttackingCombatantInfo*>, std::_Select1st<std::pair<CThingCreatureBase* const, CAttackingCombatantInfo*> >, std::less<CThingCreatureBase*>, std::allocator<std::pair<CThingCreatureBase* const, CAttackingCombatantInfo*> > >::clear() -- STLport 4.6 clear(): when non-empty, _M_erase from the root, header leftmost/rightmost back to the header, root 0, count 0
// Leaf: ?clear@?$_Rb_tree@PAVCThingCreatureBase@@U?$pair@QAVCThingCreatureBase@@PAVCAttackingCombatantInfo@@@std@@U?$_Select1st@U?$pair@QAVCThingCreatureBase@@PAVCAttackingCombatantInfo@@@std@@@3@U?$less@PAVCThingCreatureBase@@@3@V?$allocator@U?$pair@QAVCThingCreatureBase@@PAVCAttackingCombatantInfo@@@std@@@3@@std@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006F0BC0, 51 bytes.
// Types (proven, agentFAM11, 2026-11-12): map<CThingCreatureBase*, CAttackingCombatantInfo*> (node 0x18 = 0x10 + 4 + 4). Sources: (1) retail call chain: the landed
//   map<CThingCreatureBase*, CAttackingCombatantInfo*>::erase(const key&) 0x006F3340 (proven types; called on the attackers map by
//   AssignAttackersToRing 0x006EBF70) calls this tree's erase(first, last) 0x006F1CF0, which clears through _M_erase 0x006EFCE0; the
//   ~_Rb_tree 0x006F1BA0, ~map 0x006F24D0 and clear 0x006F0BC0 call 0x006EFCE0, and CCombatWheel::PopulateCombatRings 0x006EC850 destroys
//   its two local attacker maps (filled by insert_unique 0x006F2A40, passed to AssignAttackersToRing) through it; (2) Ego_r.pdb
//   CDefendingCombatantInfo::CCombatRing::AssignAttackersToRing(..., map<CThingCreatureBase*, CAttackingCombatantInfo*>&) and
//   CCombatWheel::Init(..., const map<CThingCreatureBase*, CAttackingCombatantInfo*>&); FableWin.pdb has ??0 / ??1 / clear / _Erase / erase
//   of this map / tree.
//   Was the placeholder std::_Rb_tree<CUnprovenKey_006efce0,CUnprovenValue_006efce0,CUnprovenKeyOfValue_006efce0,std::less<CUnprovenKey_006efce0>,std::allocator<CUnprovenValue_006efce0> >::clear (leaf ?clear@?$_Rb_tree@UCUnprovenKey_006efce0@@UCUnprovenValue_006efce0@@UCUnprovenKeyOfValue_006efce0@@U?$less@UCUnprovenKey_006efce0@@@std@@V?$allocator@UCUnprovenValue_006efce0@@@5@@std@@QAEXXZ); name-only change: the same member, the per-instantiation placeholder types replaced by
//   the proven ones.
// Model: the shared stlport/fable_map.h (STLport 4.6 _Rb_tree / map; FABLE_RETAIL_STLP_MAP); the explicit instantiation emits
//   the member as its own COMDAT, as retail kept every used inline member (no /OPT:REF).
#define FABLE_RETAIL_STLP_MAP
class CThingCreatureBase;
class CAttackingCombatantInfo;
#include "stlport/fable_map.h"

template void std::_Rb_tree<CThingCreatureBase*, std::pair<CThingCreatureBase* const, CAttackingCombatantInfo*>, std::_Select1st<std::pair<CThingCreatureBase* const, CAttackingCombatantInfo*> >, std::less<CThingCreatureBase*>, std::allocator<std::pair<CThingCreatureBase* const, CAttackingCombatantInfo*> > >::clear();
