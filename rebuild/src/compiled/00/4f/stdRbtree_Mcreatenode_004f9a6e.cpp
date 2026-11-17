// std::_Rb_tree<EHeroExperienceStatCategory, pair<EHeroExperienceStatCategory const, float>, _Select1st<..>, less<EHeroExperienceStatCategory>, allocator<..> >::_M_create_node(const value_type& x)
//   -- STLport 4.6 _tree.h: allocate one tree node (the 0x10-byte links plus value_type) and copy-construct the value into it
//   (`_Construct`, placement new, null-tested); `this` is never read
// Leaf: ?_M_create_node@?$_Rb_tree@W4EHeroExperienceStatCategory@@U?$pair@$$CBW4EHeroExperienceStatCategory@@M@std@@U?$_Select1st@U?$pair@$$CBW4EHeroExperienceStatCategory@@M@std@@@3@U?$less@W4EHeroExperienceStatCategory@@@3@V?$allocator@U?$pair@$$CBW4EHeroExperienceStatCategory@@M@std@@@3@@std@@IAEPAU?$_Rb_tree_node@U?$pair@$$CBW4EHeroExperienceStatCategory@@M@std@@@2@ABU?$pair@$$CBW4EHeroExperienceStatCategory@@M@2@@Z
// Flags: /d2inlS20
// Inline-budget: /d2inlS20 -- STLport's inline _Construct (placement new of the pair copy) inside _M_create_node: retail expands it in this
//   size TU; the same value as the byte twin 0x004615C3 (NF61 hand-proof there: plain VC7.1 /Os keeps it a call, S20 is the smallest value
//   that matches the family).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004F9A6E, 34 bytes, size TU, thiscall `ret 4`.
// Identity (NF92, q11206 follow-on): the protected STLport member (IAE) of the map<EHeroExperienceStatCategory, float> tree (Ego_r.pdb
//   CHeroStatIncreaseData::StatIncreases); its _M_copy 0x004F99DE (re-authored by NF92 on the spelling the ported 0x004F9960 binds) calls
//   it twice under exactly this spelling; the body is the 0x004615C3 shape (24-byte node, 8-byte pair copy). Was the stand-in
//   `_global::RbTree_CreateNode_sz0x18_004f9a6e` (leaf TreeNode_Allocate_24Byte, a local payload model). Descriptive-by-library (retail is
//   STLport 4.6; both PDBs are Dinkumware builds).
#pragma optimize("s",on)
#include "stlport/fable_map.h"
#include "engine/EHeroExperienceStatCategory.h"
namespace std {
template _Rb_tree_node<pair<EHeroExperienceStatCategory const, float> >* _Rb_tree<EHeroExperienceStatCategory, pair<EHeroExperienceStatCategory const, float>, _Select1st<pair<EHeroExperienceStatCategory const, float> >, less<EHeroExperienceStatCategory >, allocator<pair<EHeroExperienceStatCategory const, float> > >::_M_create_node(const pair<EHeroExperienceStatCategory const, float>&);
}
