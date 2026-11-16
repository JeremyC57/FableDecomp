// std::map<unsigned long, NUISystem::CUIState>::insert(const value_type& v) -- forward to the tree's insert_unique and return its
// (iterator, bool) pair through the hidden pointer
// Leaf: ?insert@?$map@KVCUIState@NUISystem@@U?$less@K@std@@V?$allocator@U?$pair@$$CBKVCUIState@NUISystem@@@std@@@4@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@U?$pair@$$CBKVCUIState@NUISystem@@@std@@U?$_Nonconst_traits@U?$pair@$$CBKVCUIState@NUISystem@@@std@@@2@@std@@_N@2@ABU?$pair@$$CBKVCUIState@NUISystem@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042D1A3, 20 bytes, size TU; calls _Rb_tree::insert_unique 0x0042CEE3.
// Identity (NF42, round 5): STLport 4.6 _map.h `pair<iterator,bool> insert(const value_type& x) { return _M_t.insert_unique(x); }`;
// the landed caller NUISystem::CManager::Action 0x0041E6D3 already binds this spelling (map<unsigned long, NUISystem::CUIState>), and
// the callee is that tree's insert_unique. Was `map<unsigned long, CAllocationNode*>::insert` on a local three-parameter model (a
// value type no binder proves); now an explicit instantiation over stlport/fable_map.h. Descriptive-by-library.
#pragma optimize("s", on)
#include "engine/NUISystem_CUIState.h"
#include "stlport/fable_map.h"
namespace std {
template pair<map<unsigned long, NUISystem::CUIState>::iterator, bool>
    map<unsigned long, NUISystem::CUIState>::insert(const map<unsigned long, NUISystem::CUIState>::value_type&);
}
