// std::_Rb_tree<NUISystem::CComponent*, pair<CComponent* const, pair<CWideString, long> >, ...>::_M_lower_bound(CComponent* const& k) const
// -- walk down from the root keeping the last node whose key is not less than k (the header when none)
// Leaf: ?_M_lower_bound@?$_Rb_tree@PAVCComponent@NUISystem@@U?$pair@QAVCComponent@NUISystem@@U?$pair@VCWideString@@J@std@@@std@@U?$_Select1st@U?$pair@QAVCComponent@NUISystem@@U?$pair@VCWideString@@J@std@@@std@@@4@U?$less@PAVCComponent@NUISystem@@@4@V?$allocator@U?$pair@QAVCComponent@NUISystem@@U?$pair@VCWideString@@J@std@@@std@@@4@@std@@ABEPAU_Rb_tree_node_base@2@ABQAVCComponent@NUISystem@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005B1DFE, thiscall `ret 4` (the key by reference; returns the node); size TU, no frame.
// Identity (NF42, round 5): the tree of CTCInventoryTrade's map<NUISystem::CComponent*, pair<CWideString, long> > (Ego_r.pdb member
// types); called by that map's operator[] 0x005B3100, which binds this spelling. STLport 4.6 _tree.c `_M_lower_bound`;
// descriptive-by-library (both PDBs are Dinkumware builds). Was the placeholder `RBTree_FindNode_Uint3(Tree*, void*, unsigned*)`;
// now an explicit instantiation over stlport/fable_map.h (FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE + _BOUNDS_BODY_PASS22, the
// _tree.c walk), like its sibling 0x005B1D38.
#pragma optimize("s", on)
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE
#define FABLE_RETAIL_STLP_TREE_BOUNDS_BODY_PASS22
#include "fable_string.h"
#include "stlport/fable_map.h"
namespace NUISystem { class CComponent; }
namespace std {
template _Rb_tree_node_base* _Rb_tree<NUISystem::CComponent*, pair<NUISystem::CComponent* const, pair<CWideString, long> >,
    _Select1st<pair<NUISystem::CComponent* const, pair<CWideString, long> > >, less<NUISystem::CComponent*>,
    allocator<pair<NUISystem::CComponent* const, pair<CWideString, long> > > >::_M_lower_bound(NUISystem::CComponent* const&) const;
}
