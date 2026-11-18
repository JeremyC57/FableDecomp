// std::_Rb_tree_iterator<pair<const EDamageAttribute, CHeroStatIncreaseData>, _Nonconst_traits<..> >::operator--() -- STLport 4.6 tree iterator
//   pre-decrement (_Rb_global::_M_decrement inlined): the header steps to the rightmost node, a node with a left child to the rightmost node
//   of that subtree, otherwise climb while the node is its parent's left child; the node is stored back once and `*this` returned
// Leaf: ??F?$_Rb_tree_iterator@U?$pair@$$CBW4EDamageAttribute@@VCHeroStatIncreaseData@@@std@@U?$_Nonconst_traits@U?$pair@$$CBW4EDamageAttribute@@VCHeroStatIncreaseData@@@std@@@2@@std@@QAEAAU01@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D9D90, 79 bytes, thiscall on the iterator, returns it in eax.
// Identity (NF92, q11207): the ported insert_unique 0x006DCE90 of the map<EDamageAttribute, CHeroStatIncreaseData> tree (Ego_r.pdb
//   CHeroExperienceDef member) calls this address (+0x70) as this iterator's operator-- under exactly this spelling, kept out of line by
//   FABLE_RETAIL_STLP_TREE_DEC_OUTOFLINE (the 0x0083AEA0 shape). Explicit instantiation of the stlport/fable_map.h member (the 0x0056CD10
//   recipe). Was the July stand-in `CRetailTreeIterator::Decrement` (a local node model shared by every body of this shape; retail has no
//   identical-code folding, so each address is one instantiation). Descriptive-by-library (STLport; both PDBs are Dinkumware builds).
#define FABLE_RETAIL_STLP_MAP
#include "stlport/fable_map.h"
#include "engine/EDamageAttribute.h"
#include "engine/CHeroStatIncreaseData.h"
template std::_Rb_tree_iterator<std::pair<const EDamageAttribute, CHeroStatIncreaseData>, std::_Nonconst_traits<std::pair<const EDamageAttribute, CHeroStatIncreaseData> > >& std::_Rb_tree_iterator<std::pair<const EDamageAttribute, CHeroStatIncreaseData>, std::_Nonconst_traits<std::pair<const EDamageAttribute, CHeroStatIncreaseData> > >::operator--();
