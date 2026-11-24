// std::_Rb_tree::erase(iterator position) -- (map<CThingBuilding*, list<CThingAICreature*> >) STLport 4.6 single-node erase: unlink the node through _Rebalance_for_erase (root / leftmost / rightmost kept), destroy its value, free it and decrement the count
// Leaf: ?erase@?$_Rb_tree@PAVCThingBuilding@@U?$pair@QAVCThingBuilding@@V?$list@PAVCThingAICreature@@V?$allocator@PAVCThingAICreature@@@std@@@std@@@std@@U?$_Select1st@U?$pair@QAVCThingBuilding@@V?$list@PAVCThingAICreature@@V?$allocator@PAVCThingAICreature@@@std@@@std@@@std@@@3@U?$less@PAVCThingBuilding@@@3@V?$allocator@U?$pair@QAVCThingBuilding@@V?$list@PAVCThingAICreature@@V?$allocator@PAVCThingAICreature@@@std@@@std@@@std@@@3@@std@@QAEXU?$_Rb_tree_iterator@U?$pair@QAVCThingBuilding@@V?$list@PAVCThingAICreature@@V?$allocator@PAVCThingAICreature@@@std@@@std@@@std@@U?$_Nonconst_traits@U?$pair@QAVCThingBuilding@@V?$list@PAVCThingAICreature@@V?$allocator@PAVCThingAICreature@@@std@@@std@@@std@@@2@@2@@Z
// Name fix (NF391, T3 nf389 tree-spelling unification): the value type is pair<CThingBuilding* const, list<..> > (QAV), as the map's own
//   value_type and every other landed member of this tree spell it (0x0051AA00 / 0x0051AAC0 / 0x00514730 / 0x0051B880 / 0x0051BF90); the
//   template arguments said pair<const CThingBuilding*, ..> (PBV). Bytes unchanged.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0051A2F0, 66 bytes (+ int3 padding to 80), thiscall `ret 4` (the iterator by value).
// Identity (proven): map<CThingBuilding*, list<CThingAICreature*> >; evidence: its only callers (0x0050045B, 0x0050050B) are in 0x005003E0, a __fastcall (creature, map&, building) helper of CWorldMap::PopulateRegionWithVillagers (ten calls) that looks the building up in the map, counts its list and either links a 12-byte list node holding the creature or erases the entry; FableWin.pdb names exactly these helpers AddToWorkBuildingList / AddToHomeBuildingList(CThingAICreature*, map<CThingBuilding*, list<CThingAICreature*> >&) (YI), and the value destructor 0x004AB4F0 frees a list of 12-byte pointer nodes
// Label: the manifest's "ListNode_RemoveAndSort" is propagated (family H065: six erase(iterator) bodies that differ only in the value
//   destructor and its node offset; retail has no identical-code folding, so each address is one instantiation).
//   STLport has two functions with this body per tree type -- `_Rb_tree::erase(iterator)` and the container's forwarding
//   `map/set::erase(iterator)` with the tree's erase inlined; these rows are modelled as the tree's.
// Callees: _Rb_global<bool>::_Rebalance_for_erase(z, root&, leftmost&, rightmost&) @ 0x0042A34E (__cdecl), the out-of-line list base destructor _List_base<CThingAICreature*>::~_List_base 0x004AB4F0 on the value (+0x14; list's `~list() {}` inline),
//   free 0x00BFEA14 (allocator<_Node>::deallocate, null-tested: the node pointer only fed a `lea`).
// NF246 (2026-11-22, the 0x004AB4F0 family, nf243 NOTES section 2): the row-local std::list / allocator / pair / less / _Rb_tree
//   models replaced by the shared STLport ones (stlport/fable_map.h FABLE_RETAIL_STLP_TREE_ERASE + _BODY, the erase(iterator) body the
//   erase(first, last) rows expand; stlport/fable_containers.h with FABLE_RETAIL_STLP_LIST_BASE_OUTOFLINE, the _List_base destructor
//   declared only): the value's destructor call binds ??1?$_List_base@PAVCThingAICreature@@..@std@@QAE@XZ (0x004AB4F0; list's own
//   ~list is the 5-byte jmp 0x004AC080).
#define FABLE_RETAIL_STLP_LIST_BASE_OUTOFLINE
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_ERASE
#define FABLE_RETAIL_STLP_TREE_ERASE_BODY
#include "stlport/fable_map.h"
class CThingBuilding;
class CThingAICreature;
template void std::_Rb_tree<CThingBuilding*, std::pair<CThingBuilding* const, std::list<CThingAICreature*> >, std::_Select1st<std::pair<CThingBuilding* const, std::list<CThingAICreature*> > >, std::less<CThingBuilding* >, std::allocator<std::pair<CThingBuilding* const, std::list<CThingAICreature*> > > >::erase(
    std::_Rb_tree<CThingBuilding*, std::pair<CThingBuilding* const, std::list<CThingAICreature*> >, std::_Select1st<std::pair<CThingBuilding* const, std::list<CThingAICreature*> > >, std::less<CThingBuilding* >, std::allocator<std::pair<CThingBuilding* const, std::list<CThingAICreature*> > > >::iterator);