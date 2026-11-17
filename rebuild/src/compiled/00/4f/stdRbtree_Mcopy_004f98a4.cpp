// std::_Rb_tree<EDamageAttribute, pair<const EDamageAttribute, CHeroStatIncreaseData>, ...>::_M_copy(_Link_type x, _Link_type p) -- STLport
//   4.6 _tree.c structural copy: clone x under p (_M_create_node 0x004F9934 per node), right subtrees by recursion, the left spine
//   iteratively
// Leaf: ?_M_copy@?$_Rb_tree@W4EDamageAttribute@@U?$pair@$$CBW4EDamageAttribute@@VCHeroStatIncreaseData@@@std@@U?$_Select1st@U?$pair@$$CBW4EDamageAttribute@@VCHeroStatIncreaseData@@@std@@@3@U?$less@W4EDamageAttribute@@@3@V?$allocator@U?$pair@$$CBW4EDamageAttribute@@VCHeroStatIncreaseData@@@std@@@3@@std@@AAEPAU?$_Rb_tree_node@U?$pair@$$CBW4EDamageAttribute@@VCHeroStatIncreaseData@@@std@@@2@PAU32@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004F98A4, 144 bytes, size TU with frame pointer.
// Identity (NF90, q11179 follow-on, claimed through the lock): the _M_copy of the map<EDamageAttribute, CHeroStatIncreaseData> tree (Ego_r.pdb
//   CHeroExperienceDef::StatIncreasePerHitType): its node callee 0x004F9934 is that tree's _M_create_node (landed under the STLport spelling,
//   whose value copy is the map<EHeroExperienceStatCategory, float> copy ctor 0x004F9960); its caller is that tree's operator= 0x004F9831
//   (NF90). Byte twin of 0x004F96D3 (NF70, the CComboMultiplierData tree). Descriptive-by-library (STLport; both PDBs are Dinkumware).
//   Was the stand-in CRetailTree_004f9934::Copy (no Leaf line; bound 0x004F9934 as ?Buynode@CRetailTree_004f9934@@QAEPAUnode@1@AAY03$$CBE@Z).
// Port (NF90): an explicit instantiation of stlport/fable_map.h's _M_copy (FABLE_RETAIL_STLP_TREE_MCOPY_BODY, the 0x004F96D3 recipe).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_TREE_MCOPY_BODY
#define FABLE_RETAIL_STLP_MAP_COPY_OUTOFLINE
#include "stlport/fable_map.h"
#include "engine/EDamageAttribute.h"
#include "engine/CHeroStatIncreaseData.h"
namespace std {
template _Rb_tree_node<pair<const EDamageAttribute, CHeroStatIncreaseData> >* _Rb_tree<EDamageAttribute, pair<const EDamageAttribute, CHeroStatIncreaseData>, _Select1st<pair<const EDamageAttribute, CHeroStatIncreaseData> >, less<EDamageAttribute>, allocator<pair<const EDamageAttribute, CHeroStatIncreaseData> > >::_M_copy(_Rb_tree_node<pair<const EDamageAttribute, CHeroStatIncreaseData> >*, _Rb_tree_node<pair<const EDamageAttribute, CHeroStatIncreaseData> >*);
}
