// std::_Rb_tree<EHeroTrainableStatType, pair<EHeroTrainableStatType const, SExpSpendUndo>, std::_Select1st<pair<EHeroTrainableStatType const, SExpSpendUndo> >, less<EHeroTrainableStatType >, allocator<pair<EHeroTrainableStatType const, SExpSpendUndo> > >::_M_create_node(const value_type& x) -- STLport 4.6 _tree.h: allocate one tree node (the 0x10-byte links plus
//   value_type) and copy-construct the value into it (`_Construct`, placement new, null-tested); `this` is never read
// Leaf: ?_M_create_node@?$_Rb_tree@W4EHeroTrainableStatType@@U?$pair@$$CBW4EHeroTrainableStatType@@USExpSpendUndo@@@std@@U?$_Select1st@U?$pair@$$CBW4EHeroTrainableStatType@@USExpSpendUndo@@@std@@@3@U?$less@W4EHeroTrainableStatType@@@3@V?$allocator@U?$pair@$$CBW4EHeroTrainableStatType@@USExpSpendUndo@@@std@@@3@@std@@IAEPAU?$_Rb_tree_node@U?$pair@$$CBW4EHeroTrainableStatType@@USExpSpendUndo@@@std@@@2@ABU?$pair@$$CBW4EHeroTrainableStatType@@USExpSpendUndo@@@2@@Z
// Flags: /d2inlS15
// Inline-budget: /d2inlS15 -- STLport's inline _Construct inside _M_create_node: retail expands it in this size TU, plain VC7.1 /Os
//   keeps it a call (0x00431471 hand-proof); here the SExpSpendUndo value, and the family value /d2inlS20 also expands the
//   pair copy, which retail keeps out of line (NF61 graded S20: DIFFER; S15: RELOCATION_MATCH).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00579F3E, 33 bytes, size TU, thiscall `ret 4`.
// NF61 (round 7, q10617): the protected STLport member (IAE), an explicit instantiation of stlport/fable_map.h _M_create_node for the
//   instance whose _M_insert 0x00579FD8 (landed by NF58, its template arguments proven there from the Ego_r.pdb member and the node
//   size) calls it with exactly this spelling; the value-type declarations are that row's. Was the stand-in `_global::RbTree_CreateNode_sz0x48_00579f3e`.
//   Descriptive-by-library (retail is STLport 4.6; both PDBs are Dinkumware builds).
#pragma optimize("s",on)
#include "stlport/fable_map.h"
#define FABLE_RETAIL_SEXPSPENDUNDO_ONLY
#include "engine/EHeroTrainableStatType.h"
#include "engine/CTCInventoryExperience.h"
namespace std {
template _Rb_tree_node<pair<EHeroTrainableStatType const, SExpSpendUndo> >* _Rb_tree<EHeroTrainableStatType, pair<EHeroTrainableStatType const, SExpSpendUndo>, _Select1st<pair<EHeroTrainableStatType const, SExpSpendUndo> >, less<EHeroTrainableStatType >, allocator<pair<EHeroTrainableStatType const, SExpSpendUndo> > >::_M_create_node(const pair<EHeroTrainableStatType const, SExpSpendUndo>&);
}
