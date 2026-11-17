// std::_Rb_tree<EHeroTrainableStatType, pair<EHeroTrainableStatType const, float>, _Select1st<pair<EHeroTrainableStatType const, float> >, less<EHeroTrainableStatType >, allocator<pair<EHeroTrainableStatType const, float> > >::_M_copy(x, p) -- STLport 4.6 _tree.c structural copy: clone x (_M_create_node 0x00462411, colour copied,
//   children cleared) under parent p, recurse into each right subtree, walk the left spine iteratively; returns the cloned top
// Leaf: ?_M_copy@?$_Rb_tree@W4EHeroTrainableStatType@@U?$pair@$$CBW4EHeroTrainableStatType@@M@std@@U?$_Select1st@U?$pair@$$CBW4EHeroTrainableStatType@@M@std@@@3@U?$less@W4EHeroTrainableStatType@@@3@V?$allocator@U?$pair@$$CBW4EHeroTrainableStatType@@M@std@@@3@@std@@AAEPAU?$_Rb_tree_node@U?$pair@$$CBW4EHeroTrainableStatType@@M@std@@@2@PAU32@0@Z
// Flags: /Oa
// Inline-budget: FABLE_FORCEINLINE _M_clone_node (overlay opt-in FABLE_RETAIL_STLP_TREE_MCOPY_BODY) -- retail expands it at both
//   clone sites (call _M_create_node, then the colour / child stores in place); VC7.1 /Os calls it out of line, also under
//   /d2inlS20 (NF61 hand-proof on 0x00432AA8: 115 v 144 bytes both ways).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00462381, 144 bytes, frame-pointer size TU (`optimize("y",off)` + `("s",on)`, catalog /Oa),
//   thiscall `ret 8`; `this` parks in the `push ecx` frame word for the two _M_create_node calls.
// NF61 (round 7, q10602): the private STLport member (AAE), an explicit instantiation of stlport/fable_map.h _M_copy (overlay opt-in
//   FABLE_RETAIL_STLP_TREE_MCOPY_BODY: STLport's _M_clone_node + _M_copy text, exceptions off) for the instance whose _M_create_node
//   0x00462411 it calls (re-authored by NF61 on the spelling NF58's _M_insert 0x004669BF binds; types proven there from the Ego_r.pdb member
//   and the node size); the value-type declarations are that row's. Was the stand-in `CRetailTree_00462411::Copy` (which bound the stand-in
//   `CRetailTree_00462411::Buynode`). Descriptive-by-library (retail is STLport 4.6; both PDBs are Dinkumware builds).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_TREE_MCOPY_BODY
#include "stlport/fable_map.h"
#include "engine/EHeroTrainableStatType.h"
namespace std {
template _Rb_tree_node<pair<EHeroTrainableStatType const, float> >* _Rb_tree<EHeroTrainableStatType, pair<EHeroTrainableStatType const, float>, _Select1st<pair<EHeroTrainableStatType const, float> >, less<EHeroTrainableStatType >, allocator<pair<EHeroTrainableStatType const, float> > >::_M_copy(_Rb_tree_node<pair<EHeroTrainableStatType const, float> >*, _Rb_tree_node<pair<EHeroTrainableStatType const, float> >*);
}
