// std::_Rb_tree<EDamageAttribute, pair<const EDamageAttribute, CHeroStatIncreaseData>, ...>::_M_erase(_Rb_tree_node* x) -- STLport 4.6
// recursive tree erase: for each node down the left spine, erase the right subtree, destroy the value
// (pair<const EDamageAttribute, CHeroStatIncreaseData>) and free the node
// Leaf: ?_M_erase@?$_Rb_tree@W4EDamageAttribute@@U?$pair@$$CBW4EDamageAttribute@@VCHeroStatIncreaseData@@@std@@U?$_Select1st@U?$pair@$$CBW4EDamageAttribute@@VCHeroStatIncreaseData@@@std@@@3@U?$less@W4EDamageAttribute@@@3@V?$allocator@U?$pair@$$CBW4EDamageAttribute@@VCHeroStatIncreaseData@@@std@@@3@@std@@AAEXPAU?$_Rb_tree_node@U?$pair@$$CBW4EDamageAttribute@@VCHeroStatIncreaseData@@@std@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E6E43, 55 bytes, a COMDAT in a size-optimised TU.
// Identity (NF90, q11179): the _M_erase of the map<EDamageAttribute, CHeroStatIncreaseData> tree (Ego_r.pdb
//   CHeroExperienceDef::StatIncreasePerHitType): the ported GFSerialiseMapBinaryIn 0x006DD1C0 (NF88) binds it so; its value destructor
//   0x004E3644 is the pair's scalar deleting destructor called with 0 (VC7.1's spelling of STLport _Destroy, NF90); its other callers are
//   that tree's clear 0x004E7749 and map::clear 0x006DC9B0 (NF90). Byte twin of 0x004E6E0C (NF81, the CComboMultiplierData tree).
//   Descriptive-by-library (STLport; both PDBs are Dinkumware). Was the propagated label LTextBinTree<LTextGroup*>::LTextTreeWalkThrough::
//   BuildTreeArray (a local CTextTreeNode / CInner model).
// Port (NF90): one explicit instantiation of the stlport/fable_map.h _M_erase (the 0x004E6E0C recipe).
#pragma optimize("s",on)
#include "stlport/fable_map.h"
#include "engine/EDamageAttribute.h"
#include "engine/CHeroStatIncreaseData.h"
namespace std {
template void _Rb_tree<EDamageAttribute, pair<const EDamageAttribute, CHeroStatIncreaseData>, _Select1st<pair<const EDamageAttribute, CHeroStatIncreaseData> >, less<EDamageAttribute>, allocator<pair<const EDamageAttribute, CHeroStatIncreaseData> > >::_M_erase(_Rb_tree_node<pair<const EDamageAttribute, CHeroStatIncreaseData> >* x);
}
