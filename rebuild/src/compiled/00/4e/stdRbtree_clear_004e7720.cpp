// std::_Rb_tree<long, pair<const long, CComboMultiplierData>, ...>::clear() -- STLport tree clear: when the tree holds nodes, erase them all (_M_erase from the root) and reset the header to an empty tree
// Leaf: ?clear@?$_Rb_tree@JU?$pair@$$CBJVCComboMultiplierData@@@std@@U?$_Select1st@U?$pair@$$CBJVCComboMultiplierData@@@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJVCComboMultiplierData@@@std@@@2@@std@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E7720, a COMDAT in a size-optimised TU (`#pragma optimize("s", on)`).
// Identity (NF78, q10949): the clear() of the map<long, CComboMultiplierData> tree (Ego_r.pdb CHeroExperienceDef::ComboMultiplierMap):
//   its callee 0x004E6E0C is that tree's _M_erase (bound so by the landed 0x006DCFA0 after NF78's port); its callers are the same
//   tree's operator= 0x004F9660 (STLport _tree.c; NF75, binds this spelling) and the member map's destructor 0x004E81E4 (called by
//   CHeroExperienceDef::~CHeroExperienceDef 0x004EBB1C). Descriptive-by-library (STLport; no PDB). Was the AutoTiny stand-in
//   _global::RbTree_Clear_004e7720 (leaf OnKill).
// Port (NF78): one explicit instantiation of the STLport clear on stlport/fable_map.h (the landed 0x00630FE0 recipe).
#pragma optimize("s", on)
#include "stlport/fable_map.h"
#include "engine/CComboMultiplierData.h"
namespace std {
template void _Rb_tree<long, pair<const long, CComboMultiplierData>, _Select1st<pair<const long, CComboMultiplierData> >, less<long>, allocator<pair<const long, CComboMultiplierData> > >::clear();
}
