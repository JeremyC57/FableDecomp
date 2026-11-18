// std::_Rb_tree<long, pair<const long, CComboMultiplierData>, ...>::_M_erase(_Rb_tree_node* x) -- STLport 4.6 recursive tree erase: for
// each node down the left spine, erase the right subtree, destroy the value (pair<const long, CComboMultiplierData>) and free the node
// Leaf: ?_M_erase@?$_Rb_tree@JU?$pair@$$CBJVCComboMultiplierData@@@std@@U?$_Select1st@U?$pair@$$CBJVCComboMultiplierData@@@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJVCComboMultiplierData@@@std@@@2@@std@@AAEXPAU?$_Rb_tree_node@U?$pair@$$CBJVCComboMultiplierData@@@std@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E6E0C, 55 bytes, a COMDAT in a size-optimised TU.
// Identity (NF81, round 7, q10985): the _M_erase of the map<long, CComboMultiplierData> tree (Ego_r.pdb CHeroExperienceDef::ComboMultiplierMap):
//   that tree's clear 0x004E7720 (NF78 q10949, RM) and the landed GFSerialiseMapBinaryIn 0x006DCFA0 (NF78, RM) both bind it so; its value
//   destructor 0x004E3625 is the pair's scalar deleting destructor called with 0 (VC7.1's spelling of STLport _Destroy). Descriptive-by-library
//   (STLport; both PDBs are Dinkumware). Was the propagated label LTextBinTree<LTextGroup*>::LTextTreeWalkThrough::BuildTreeArray (a local
//   CTextTreeNode / CInner model).
// Port (NF81): one explicit instantiation of the stlport/fable_map.h _M_erase (the 0x005BA78B recipe).
#pragma optimize("s",on)
#include "stlport/fable_map.h"
#include "engine/CComboMultiplierData.h"
namespace std {
template void _Rb_tree<long, pair<const long, CComboMultiplierData>, _Select1st<pair<const long, CComboMultiplierData> >, less<long>, allocator<pair<const long, CComboMultiplierData> > >::_M_erase(_Rb_tree_node<pair<const long, CComboMultiplierData> >* x);
}
