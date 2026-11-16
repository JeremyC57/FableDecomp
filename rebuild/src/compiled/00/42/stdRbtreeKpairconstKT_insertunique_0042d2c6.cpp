// std::_Rb_tree<K, pair<const K, T>, ...>::insert_unique(iterator position, const value_type& v) -- STLport 4.6 hinted unique insert of map<unsigned long, NUISystem::CUIState> (NUISystem::CComponent::States): begin / end / interior hint cases, falling back on the unhinted insert_unique when the hint does not bracket v
// Leaf: ?insert_unique@?$_Rb_tree@KU?$pair@$$CBKVCUIState@NUISystem@@@std@@U?$_Select1st@U?$pair@$$CBKVCUIState@NUISystem@@@std@@@2@U?$less@K@2@V?$allocator@U?$pair@$$CBKVCUIState@NUISystem@@@std@@@2@@std@@QAE?AU?$_Rb_tree_iterator@U?$pair@$$CBKVCUIState@NUISystem@@@std@@U?$_Nonconst_traits@U?$pair@$$CBKVCUIState@NUISystem@@@std@@@2@@2@U32@ABU?$pair@$$CBKVCUIState@NUISystem@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0042D2C6, 319 bytes, thiscall `ret 0Ch` (hidden iterator result, position, v);
//   size-optimised tree TU with frame pointer (odd entry).
// Identity (proven): _Rb_tree<unsigned long, pair<unsigned long const, NUISystem::CUIState >, ...>::insert_unique(iterator, const value_type&) of
//   map<unsigned long, NUISystem::CUIState> (NUISystem::CComponent::States); evidence: callers are the map::insert forwarder 0x0042D4D3 (from the landed CUIState-defaulting map::operator[] 0x0042D5B1) and CComponent::GetDataFromDef 0x005331A0 and the inventory-map / quest-list builders; the 0x24-byte mapped CUIState (CUIState() 0x0052E5D0 in the operator[]) makes the node 0x38 bytes, and Ego_r.pdb's only map with a CUIState value is NUISystem::CComponent::States, map<unsigned long, NUISystem::CUIState> (unsigned key compare); node 0x38 bytes.
// Label: the manifest's "CMemoryAllocatorVariableSize_FindAllocationNode" is propagated across family H039 (six hinted map inserts).
// Callees (this instantiation): _M_insert 0x0042CB08 (node creator 0x0042BDF0, `push 38h`: 0x10 + sizeof(value_type)),
//   insert_unique(v) 0x0042CEE3; _Rb_global<bool>::_M_increment 0x004292C0 / _M_decrement 0x004294E2 (cdecl, shared).
// Shape: STLport 4.6 _tree.c verbatim; the _Rb_global step functions are defined in the TU (as STLport's template
//   statics are) so VC7.1 sees they write no memory and keeps v's key in edi across the `++after` call (declared-only
//   steps reload it: 317v319); the node key is spelled `node->_M_value_field.first` (the _KeyOfValue temporary loads it
//   before v's key), as in the landed /O2 sibling 0x00AD5030.
// NF42 (round 5): the local STLport tree model (which declared _M_insert public, binding ?_M_insert@..@QAE..) replaced by the shared
// stlport/fable_map.h (FABLE_RETAIL_STLP_MAP + FABLE_RETAIL_STLP_TREE_INSERT_UNIQUE_HINT_BODY, the same _tree.c text): _M_insert now
// binds STLport's private ?_M_insert@..@AAE.. at 0x0042CB08.
#pragma optimize("y", off)
#pragma optimize("s", on)
#include "engine/NUISystem_CUIState.h"
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_INSERT_UNIQUE_HINT_BODY
#include "stlport/fable_map.h"
namespace std {
template _Rb_tree<unsigned long, pair<const unsigned long, NUISystem::CUIState>, _Select1st<pair<const unsigned long, NUISystem::CUIState> >, less<unsigned long>, allocator<pair<const unsigned long, NUISystem::CUIState> > >::iterator _Rb_tree<unsigned long, pair<const unsigned long, NUISystem::CUIState>, _Select1st<pair<const unsigned long, NUISystem::CUIState> >, less<unsigned long>, allocator<pair<const unsigned long, NUISystem::CUIState> > >::insert_unique(_Rb_tree<unsigned long, pair<const unsigned long, NUISystem::CUIState>, _Select1st<pair<const unsigned long, NUISystem::CUIState> >, less<unsigned long>, allocator<pair<const unsigned long, NUISystem::CUIState> > >::iterator, const pair<const unsigned long, NUISystem::CUIState>&);
}
