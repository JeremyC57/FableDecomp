// std::_Rb_tree<unsigned long, pair<const unsigned long, NUISystem::CUIState>, ...>::_M_lower_bound(const unsigned long& k) const -- walk
// down from the root keeping the last node whose key is not less than k (the header when none)
// Leaf: ?_M_lower_bound@?$_Rb_tree@KU?$pair@$$CBKVCUIState@NUISystem@@@std@@U?$_Select1st@U?$pair@$$CBKVCUIState@NUISystem@@@std@@@2@U?$less@K@2@V?$allocator@U?$pair@$$CBKVCUIState@NUISystem@@@std@@@2@@std@@ABEPAU_Rb_tree_node_base@2@ABK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042AD6C, thiscall `ret 4` (the key by reference; returns the node), size TU.
// Identity (NF42, round 5): the tree of map<unsigned long, NUISystem::CUIState> (CUIState from engine/NUISystem_CUIState.h); called by
// that map's operator[] 0x0042D5B1 (landed, binds this spelling) and two unlanded sites (0x0042B830, 0x0042BFCF). STLport 4.6 _tree.c
// walk; descriptive-by-library. The return type is stlport/fable_map.h's `_Rb_tree_node_base*` (_Base_ptr), as the 13 landed
// _M_lower_bound bindings spell it; the queue claim's `_Rb_tree_node<..>*` return is not used (REJECTED.tsv). Was the placeholder
// `BinarySearchTreeFindEqualOrGreater` on local node / tree models; now an explicit instantiation over the shared headers.
#pragma optimize("s", on)
#include "engine/NUISystem_CUIState.h"
#define FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE
#define FABLE_RETAIL_STLP_TREE_BOUNDS_BODY_PASS22
#include "stlport/fable_map.h"
namespace std {
template _Rb_tree_node_base* _Rb_tree<unsigned long, pair<const unsigned long, NUISystem::CUIState>,
    _Select1st<pair<const unsigned long, NUISystem::CUIState> >, less<unsigned long>,
    allocator<pair<const unsigned long, NUISystem::CUIState> > >::_M_lower_bound(const unsigned long&) const;
}
