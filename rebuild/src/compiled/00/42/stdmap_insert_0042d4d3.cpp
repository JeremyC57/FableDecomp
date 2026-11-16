// std::map<unsigned long, NUISystem::CUIState>::insert(iterator position, const value_type& x) -- hinted insert: forward to the tree's
// insert_unique(position, x) and return its iterator
// Leaf: ?insert@?$map@KVCUIState@NUISystem@@U?$less@K@std@@V?$allocator@U?$pair@$$CBKVCUIState@NUISystem@@@std@@@4@@std@@QAE?AU?$_Rb_tree_iterator@U?$pair@$$CBKVCUIState@NUISystem@@@std@@U?$_Nonconst_traits@U?$pair@$$CBKVCUIState@NUISystem@@@std@@@2@@2@U32@ABU?$pair@$$CBKVCUIState@NUISystem@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042D4D3, 29 bytes, thiscall `ret 0Ch` (hidden result, position, x): the iterator copied into a
// reserved argument slot, `call 0x0042D2C6` (_Rb_tree::insert_unique(iterator, const value_type&)).
// Identity (NF42, round 5): STLport 4.6 _map.h `iterator insert(iterator, const value_type&) { return _M_t.insert_unique(position, x); }`;
// the only caller is map<unsigned long, CUIState>::operator[] 0x0042D5B1 (landed, binds this spelling). Descriptive-by-library
// (STLport 4.6.2; both PDBs are Dinkumware builds). Was the placeholder `CForwarder_0042d4d3::Forward`; now an explicit
// instantiation over stlport/fable_map.h, like the twin 0x005B31CF.
#pragma optimize("s", on)
#include "engine/NUISystem_CUIState.h"
#define FABLE_RETAIL_STLP_MAP
#include "stlport/fable_map.h"
namespace std {
template map<unsigned long, NUISystem::CUIState>::iterator
    map<unsigned long, NUISystem::CUIState>::insert(iterator, const value_type&);
}
