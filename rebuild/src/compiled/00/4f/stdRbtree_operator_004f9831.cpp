// std::_Rb_tree<EDamageAttribute, pair<const EDamageAttribute, CHeroStatIncreaseData>, ...>::operator=(const _Rb_tree&) -- STLport 4.6
//   _tree.c tree assignment: unless self-assigned, clear 0x004E7749, zero the count, then reset the header to an empty tree or _M_copy
//   0x004F98A4 the source root under the header, walk leftmost / rightmost, copy the count
// Leaf: ??4?$_Rb_tree@W4EDamageAttribute@@U?$pair@$$CBW4EDamageAttribute@@VCHeroStatIncreaseData@@@std@@U?$_Select1st@U?$pair@$$CBW4EDamageAttribute@@VCHeroStatIncreaseData@@@std@@@3@U?$less@W4EDamageAttribute@@@3@V?$allocator@U?$pair@$$CBW4EDamageAttribute@@VCHeroStatIncreaseData@@@std@@@3@@std@@QAEAAV01@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004F9831, 115 bytes, size TU (`and dword ptr [..],0`), thiscall `ret 4`.
// Identity (NF90, q11179 follow-on, claimed through the lock): the operator= of the map<EDamageAttribute, CHeroStatIncreaseData> tree
//   (Ego_r.pdb CHeroExperienceDef::StatIncreasePerHitType; CHeroExperienceDef::operator= 0x004F914A calls it): its clear callee 0x004E7749
//   is that tree's clear (NF90) and its _M_copy callee 0x004F98A4 calls that tree's _M_create_node 0x004F9934 (landed under the STLport
//   spelling). Byte twin of 0x004F9660 (NF75, the CComboMultiplierData tree operator=). Descriptive-by-library (retail is STLport 4.6;
//   both PDBs are Dinkumware builds). Was the stand-in CRetailTree_004f98a4::operator= (bound ?Clear@CRetailTree_004f98a4 /
//   ?Copy@CRetailTree_004f98a4).
// Port (NF90): an explicit instantiation of stlport/fable_map.h operator= (FABLE_RETAIL_STLP_TREE_ASSIGN_BODY, the 0x004F9660 recipe).
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_TREE_ASSIGN_BODY
#include "stlport/fable_map.h"
#include "engine/EDamageAttribute.h"
#include "engine/CHeroStatIncreaseData.h"
namespace std {
template _Rb_tree<EDamageAttribute, pair<const EDamageAttribute, CHeroStatIncreaseData>, _Select1st<pair<const EDamageAttribute, CHeroStatIncreaseData> >, less<EDamageAttribute>, allocator<pair<const EDamageAttribute, CHeroStatIncreaseData> > >& _Rb_tree<EDamageAttribute, pair<const EDamageAttribute, CHeroStatIncreaseData>, _Select1st<pair<const EDamageAttribute, CHeroStatIncreaseData> >, less<EDamageAttribute>, allocator<pair<const EDamageAttribute, CHeroStatIncreaseData> > >::operator=(const _Rb_tree<EDamageAttribute, pair<const EDamageAttribute, CHeroStatIncreaseData>, _Select1st<pair<const EDamageAttribute, CHeroStatIncreaseData> >, less<EDamageAttribute>, allocator<pair<const EDamageAttribute, CHeroStatIncreaseData> > >&);
}
