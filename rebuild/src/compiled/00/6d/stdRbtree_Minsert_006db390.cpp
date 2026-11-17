// std::_Rb_tree<EHeroExperienceStatCategory, pair<EHeroExperienceStatCategory const, float>, _Select1st<pair<EHeroExperienceStatCategory const, float> >, less<EHeroExperienceStatCategory >, allocator<pair<EHeroExperienceStatCategory const, float> > >::_M_insert(x, y, v, w) -- STLport 4.6 _tree.c: allocate and copy-construct the node (_M_create_node, inline here), link it as y's left or right child (fixing root / leftmost / rightmost), rebalance (_Rb_global<bool>::_Rebalance 0x0042971D), count it, return the iterator
// Leaf: ?_M_insert@?$_Rb_tree@W4EHeroExperienceStatCategory@@U?$pair@$$CBW4EHeroExperienceStatCategory@@M@std@@U?$_Select1st@U?$pair@$$CBW4EHeroExperienceStatCategory@@M@std@@@3@U?$less@W4EHeroExperienceStatCategory@@@3@V?$allocator@U?$pair@$$CBW4EHeroExperienceStatCategory@@M@std@@@3@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@$$CBW4EHeroExperienceStatCategory@@M@std@@U?$_Nonconst_traits@U?$pair@$$CBW4EHeroExperienceStatCategory@@M@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBW4EHeroExperienceStatCategory@@M@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006DB390, 191 bytes, thiscall `ret 14h` (hidden iterator result + x, y, &v, w); /O2 TU.
// Identity (NF58, round 7): the map<EHeroExperienceStatCategory, float> tree; Ego_r.pdb members of this type:
//   CHeroStatIncreaseData::StatIncreases. Landed callers of this address: 0x006DB610 _unattributed::InsertUnique_CRetailTree_006db610.
//   STLport 4.6.2 _tree.c (private member), descriptive-by-library (both PDBs are Dinkumware builds). Was the stand-in
//   `_unattributed::Insert_CRetailTree_006db390`; now the explicit instantiation of stlport/fable_map.h
//   FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY (NF56's 0x0069D520 pattern; malloc 0x00BFEA0E is the STLport node allocator).
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "stlport/fable_map.h"
#include "engine/EHeroExperienceStatCategory.h"
namespace std {
template _Rb_tree<EHeroExperienceStatCategory, pair<EHeroExperienceStatCategory const, float>, _Select1st<pair<EHeroExperienceStatCategory const, float> >, less<EHeroExperienceStatCategory >, allocator<pair<EHeroExperienceStatCategory const, float> > >::iterator _Rb_tree<EHeroExperienceStatCategory, pair<EHeroExperienceStatCategory const, float>, _Select1st<pair<EHeroExperienceStatCategory const, float> >, less<EHeroExperienceStatCategory >, allocator<pair<EHeroExperienceStatCategory const, float> > >::_M_insert(_Rb_tree_node_base*, _Rb_tree_node_base*, const pair<EHeroExperienceStatCategory const, float>&, _Rb_tree_node_base*);
}
