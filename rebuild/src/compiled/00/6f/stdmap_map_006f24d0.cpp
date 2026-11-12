// std::map<CThingCreatureBase*, CAttackingCombatantInfo*>::~map() -- STLport 4.6 container destructor (implicit): the member _Rb_tree's destructor inlined -- clear(), then free the header node
// Leaf: ??1?$map@PAVCThingCreatureBase@@PAVCAttackingCombatantInfo@@U?$less@PAVCThingCreatureBase@@@std@@V?$allocator@U?$pair@QAVCThingCreatureBase@@PAVCAttackingCombatantInfo@@@std@@@4@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006F24D0, 66 bytes.
// Types (proven, agentFAM11, 2026-11-12): map<CThingCreatureBase*, CAttackingCombatantInfo*> (node 0x18 = 0x10 + 4 + 4). Sources: (1) retail call chain: the landed
//   map<CThingCreatureBase*, CAttackingCombatantInfo*>::erase(const key&) 0x006F3340 (proven types; called on the attackers map by
//   AssignAttackersToRing 0x006EBF70) calls this tree's erase(first, last) 0x006F1CF0, which clears through _M_erase 0x006EFCE0; the
//   ~_Rb_tree 0x006F1BA0, ~map 0x006F24D0 and clear 0x006F0BC0 call 0x006EFCE0, and CCombatWheel::PopulateCombatRings 0x006EC850 destroys
//   its two local attacker maps (filled by insert_unique 0x006F2A40, passed to AssignAttackersToRing) through it; (2) Ego_r.pdb
//   CDefendingCombatantInfo::CCombatRing::AssignAttackersToRing(..., map<CThingCreatureBase*, CAttackingCombatantInfo*>&) and
//   CCombatWheel::Init(..., const map<CThingCreatureBase*, CAttackingCombatantInfo*>&); FableWin.pdb has ??0 / ??1 / clear / _Erase / erase
//   of this map / tree.
//   Was the placeholder CUnprovenContainer_006efce0::~CUnprovenContainer_006efce0 (leaf ??1CUnprovenContainer_006efce0@@QAE@XZ); name-only change: the same member, the per-instantiation placeholder types replaced by
//   the proven ones.
// Model: the shared stlport/fable_map.h (STLport 4.6 _Rb_tree / map; FABLE_RETAIL_STLP_MAP); the explicit instantiation emits
//   the member as its own COMDAT, as retail kept every used inline member (no /OPT:REF).
#define FABLE_RETAIL_STLP_MAP
class CThingCreatureBase;
class CAttackingCombatantInfo;
#include "stlport/fable_map.h"

// The implicit ~map (STLport declares none) inlines ~_Rb_tree: clear(), then free the header node. Retail emitted it out
//   of line; this emitter (not part of the graded body) makes the TU emit that COMDAT.
void DestroyMapOfCThingCreatureBaseCAttackingCombatantInfo(std::map<CThingCreatureBase*, CAttackingCombatantInfo*>* container) { container->~map(); }
