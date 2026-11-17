// std::_Rb_tree<EDamageAttribute, pair<const EDamageAttribute, CHeroStatIncreaseData>, ...>::insert_unique(const value_type& v) -- STLport 4.6
//   _tree.c unique insert: descend comparing keys, step back once through the out-of-line decrement unless at begin(), insert through
//   _M_insert when the key is new; {iterator, inserted} through the hidden pointer
// Leaf: ?insert_unique@?$_Rb_tree@W4EDamageAttribute@@U?$pair@$$CBW4EDamageAttribute@@VCHeroStatIncreaseData@@@std@@U?$_Select1st@U?$pair@$$CBW4EDamageAttribute@@VCHeroStatIncreaseData@@@std@@@3@U?$less@W4EDamageAttribute@@@3@V?$allocator@U?$pair@$$CBW4EDamageAttribute@@VCHeroStatIncreaseData@@@std@@@3@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@U?$pair@$$CBW4EDamageAttribute@@VCHeroStatIncreaseData@@@std@@U?$_Nonconst_traits@U?$pair@$$CBW4EDamageAttribute@@VCHeroStatIncreaseData@@@std@@@2@@std@@_N@2@ABU?$pair@$$CBW4EDamageAttribute@@VCHeroStatIncreaseData@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006DCE90, 180 bytes, thiscall `ret 8` (hidden pair result + value reference); the enum keys
//   compare signed (`setl`).
// Identity (NF90, q11181): the insert_unique of the map<EDamageAttribute, CHeroStatIncreaseData> tree (Ego_r.pdb
//   CHeroExperienceDef::StatIncreasePerHitType): the ported GFSerialiseMapBinaryIn 0x006DD1C0 (NF88) binds it so from the shared body
//   (map::insert = _Rb_tree::insert_unique); its _M_insert callee 0x006DCBF0 is that tree's _M_insert (landed under the STLport spelling)
//   and its decrement callee 0x006D9D90 is that tree's iterator operator--. Same shape as the twin 0x006DCDD0 (the map<long,
//   CComboMultiplierData> insert_unique that 0x006DCFA0 calls). Descriptive-by-library (STLport; both PDBs are Dinkumware). Was the stand-in
//   _unattributed::InsertUnique_CRetailTree_006dce90 (a local SNode / SIterator model).
// Port (NF90): one explicit instantiation of the stlport/fable_map.h insert_unique body (FABLE_RETAIL_STLP_MAP) with the decrement kept out
//   of line (FABLE_RETAIL_STLP_TREE_DEC_OUTOFLINE), the 0x00756250 recipe.
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_DEC_OUTOFLINE
#include "stlport/fable_map.h"
#include "engine/EDamageAttribute.h"
#include "engine/CHeroStatIncreaseData.h"
namespace std {
template pair<_Rb_tree<EDamageAttribute, pair<const EDamageAttribute, CHeroStatIncreaseData>, _Select1st<pair<const EDamageAttribute, CHeroStatIncreaseData> >, less<EDamageAttribute>, allocator<pair<const EDamageAttribute, CHeroStatIncreaseData> > >::iterator, bool> _Rb_tree<EDamageAttribute, pair<const EDamageAttribute, CHeroStatIncreaseData>, _Select1st<pair<const EDamageAttribute, CHeroStatIncreaseData> >, less<EDamageAttribute>, allocator<pair<const EDamageAttribute, CHeroStatIncreaseData> > >::insert_unique(const pair<const EDamageAttribute, CHeroStatIncreaseData>&);
}
