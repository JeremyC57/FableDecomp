// std::_Rb_tree<EHeroExperienceStatCategory, pair<EHeroExperienceStatCategory const, float>, _Select1st<..>, less<EHeroExperienceStatCategory>, allocator<..> >::_M_copy(x, p)
//   -- STLport 4.6 _tree.c structural copy: clone x (_M_create_node 0x004F9A6E, colour copied, children cleared) under parent p, recurse
//   into each right subtree, walk the left spine iteratively; returns the cloned top
// Leaf: ?_M_copy@?$_Rb_tree@W4EHeroExperienceStatCategory@@U?$pair@$$CBW4EHeroExperienceStatCategory@@M@std@@U?$_Select1st@U?$pair@$$CBW4EHeroExperienceStatCategory@@M@std@@@3@U?$less@W4EHeroExperienceStatCategory@@@3@V?$allocator@U?$pair@$$CBW4EHeroExperienceStatCategory@@M@std@@@3@@std@@AAEPAU?$_Rb_tree_node@U?$pair@$$CBW4EHeroExperienceStatCategory@@M@std@@@2@PAU32@0@Z
// Flags: /Oa
// Inline-budget: FABLE_FORCEINLINE _M_clone_node (opt-in FABLE_RETAIL_STLP_TREE_MCOPY_BODY) -- retail expands it at both clone sites, as in
//   the byte twin 0x00461533 (NF61 hand-proof on 0x00432AA8: VC7.1 /Os calls it out of line, also under /d2inlS20).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004F99DE, 144 bytes, frame-pointer size TU (`optimize("y",off)` + `("s",on)`, catalog /Oa),
//   thiscall `ret 8`; `this` parks in the `push ecx` frame word for the two _M_create_node calls.
// Identity (NF92, q11206): the private STLport member (AAE) of the map<EHeroExperienceStatCategory, float> tree (Ego_r.pdb
//   CHeroStatIncreaseData::StatIncreases): the ported map copy constructor 0x004F9960 binds this address under this spelling (+0x42), and
//   the body is byte-for-byte the shape of 0x00461533 / 0x004F97B3's _M_copy (the map<EDamageAttribute, float> sibling). Explicit
//   instantiation of stlport/fable_map.h _M_copy. Was the July stand-in `CRetailTree_004f9a6e::Copy` (a local tree model; it bound
//   `CRetailTree_004f9a6e::Buynode`). Descriptive-by-library (retail is STLport 4.6; both PDBs are Dinkumware builds).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_TREE_MCOPY_BODY
#include "stlport/fable_map.h"
#include "engine/EHeroExperienceStatCategory.h"
namespace std {
template _Rb_tree_node<pair<EHeroExperienceStatCategory const, float> >* _Rb_tree<EHeroExperienceStatCategory, pair<EHeroExperienceStatCategory const, float>, _Select1st<pair<EHeroExperienceStatCategory const, float> >, less<EHeroExperienceStatCategory >, allocator<pair<EHeroExperienceStatCategory const, float> > >::_M_copy(_Rb_tree_node<pair<EHeroExperienceStatCategory const, float> >*, _Rb_tree_node<pair<EHeroExperienceStatCategory const, float> >*);
}
