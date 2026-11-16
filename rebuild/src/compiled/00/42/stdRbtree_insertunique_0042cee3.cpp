// std::_Rb_tree<unsigned long, pair<const unsigned long, NUISystem::CUIState>, ...>::insert_unique(const value_type& v) -- STLport 4.6
// _tree.c unique insert: descend on key < node key, step back with _Rb_global<bool>::_M_decrement (0x004294E2) unless at begin(), then
// _M_insert (0x0042CB08) when the predecessor's key is less; pair<iterator, bool> through the hidden pointer
// Leaf: ?insert_unique@?$_Rb_tree@KU?$pair@$$CBKVCUIState@NUISystem@@@std@@U?$_Select1st@U?$pair@$$CBKVCUIState@NUISystem@@@std@@@2@U?$less@K@2@V?$allocator@U?$pair@$$CBKVCUIState@NUISystem@@@std@@@2@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@U?$pair@$$CBKVCUIState@NUISystem@@@std@@U?$_Nonconst_traits@U?$pair@$$CBKVCUIState@NUISystem@@@std@@@2@@std@@_N@2@ABU?$pair@$$CBKVCUIState@NUISystem@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042CEE3, 134 bytes, thiscall, frame-pointer size TU.
// Identity (NF42, round 5): the map<unsigned long, NUISystem::CUIState> tree (Codex pass 22: map value type from both PDBs; the landed
// _M_clone_node 0x0042BE11 and insert_unique(iterator, ..) 0x0042D2C6 of the same tree bind the same chain); called by
// map::insert(const value_type&) 0x0042D1A3 and the hinted insert_unique 0x0042D2C6 (binds this spelling). STLport 4.6.2 source,
// descriptive-by-library. Was the placeholder `CRetailTree_ulongkey_ins0042cb08::insert` (Dinkumware-shaped local model); now an
// explicit instantiation over stlport/fable_map.h (FABLE_RETAIL_STLP_MAP body; FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ: out-of-line
// _M_decrement).
#pragma optimize("y", off)
#pragma optimize("s", on)
#include "engine/NUISystem_CUIState.h"
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ
#include "stlport/fable_map.h"
namespace std {
template pair<_Rb_tree<unsigned long, pair<const unsigned long, NUISystem::CUIState>, _Select1st<pair<const unsigned long, NUISystem::CUIState> >, less<unsigned long>, allocator<pair<const unsigned long, NUISystem::CUIState> > >::iterator, bool> _Rb_tree<unsigned long, pair<const unsigned long, NUISystem::CUIState>, _Select1st<pair<const unsigned long, NUISystem::CUIState> >, less<unsigned long>, allocator<pair<const unsigned long, NUISystem::CUIState> > >::insert_unique(const pair<const unsigned long, NUISystem::CUIState>&);
}
