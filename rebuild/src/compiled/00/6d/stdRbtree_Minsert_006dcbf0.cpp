// std::_Rb_tree<EDamageAttribute, pair<const EDamageAttribute, CHeroStatIncreaseData>, ...>::_M_insert(_Rb_tree_node_base* x,
//   _Rb_tree_node_base* y, const value_type& v, _Rb_tree_node_base* w) -- STLport 4.6 node insert: create a node for v, link it left or
//   right of y, fix root / leftmost / rightmost, rebalance, count
// Leaf: ?_M_insert@?$_Rb_tree@W4EDamageAttribute@@U?$pair@$$CBW4EDamageAttribute@@VCHeroStatIncreaseData@@@std@@U?$_Select1st@U?$pair@$$CBW4EDamageAttribute@@VCHeroStatIncreaseData@@@std@@@3@U?$less@W4EDamageAttribute@@@3@V?$allocator@U?$pair@$$CBW4EDamageAttribute@@VCHeroStatIncreaseData@@@std@@@3@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@$$CBW4EDamageAttribute@@VCHeroStatIncreaseData@@@std@@U?$_Nonconst_traits@U?$pair@$$CBW4EDamageAttribute@@VCHeroStatIncreaseData@@@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBW4EDamageAttribute@@VCHeroStatIncreaseData@@@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006DCBF0, 202 bytes (+ int3 padding to 208), thiscall `ret 14h` (the hidden iterator result
//   pointer first, then x, y, &v, w).
// Identity (proven, landed): map<EDamageAttribute, CHeroStatIncreaseData> (Ego_r.pdb CHeroExperienceDef::StatIncreasePerHitType); the value
//   copy is map<EHeroExperienceStatCategory, float>::map(const map&) 0x004F9960 (NF90, q11180) twice (the inlined _M_create_node on both
//   paths), _Rb_global<bool>::_Rebalance 0x0042971D.
// Access (NF90, q11181 follow-on): STLport declares _M_insert private (AAE), as the twin 0x006DCB20 (NF62) and 119 landed bindings spell it;
//   the old all-local model made it public (QAE). The insert_unique 0x006DCE90 (NF90) binds the AAE spelling.
// Port (NF90): an explicit instantiation on stlport/fable_map.h (ORIGINAL_INSERT_BODY + MAP_COPY_OUTOFLINE), the 0x006DCB20 recipe. Was a
//   local allocator / pair / _Rb_tree model (family H011).
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#define FABLE_RETAIL_STLP_MAP_COPY_OUTOFLINE
#include "stlport/fable_map.h"
#include "stlport/fable_containers.h"
#include "engine/EDamageAttribute.h"
#include "engine/CHeroStatIncreaseData.h"
namespace std {
template _Rb_tree<EDamageAttribute, pair<const EDamageAttribute, CHeroStatIncreaseData>, _Select1st<pair<const EDamageAttribute, CHeroStatIncreaseData> >, less<EDamageAttribute>, allocator<pair<const EDamageAttribute, CHeroStatIncreaseData> > >::iterator _Rb_tree<EDamageAttribute, pair<const EDamageAttribute, CHeroStatIncreaseData>, _Select1st<pair<const EDamageAttribute, CHeroStatIncreaseData> >, less<EDamageAttribute>, allocator<pair<const EDamageAttribute, CHeroStatIncreaseData> > >::_M_insert(_Rb_tree_node_base*, _Rb_tree_node_base*, const pair<const EDamageAttribute, CHeroStatIncreaseData>&, _Rb_tree_node_base*);
}
