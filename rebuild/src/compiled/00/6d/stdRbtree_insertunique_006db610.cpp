// std::_Rb_tree<EHeroExperienceStatCategory, pair<EHeroExperienceStatCategory const, float>, _Select1st<pair<EHeroExperienceStatCategory const, float> >, less<EHeroExperienceStatCategory >, allocator<pair<EHeroExperienceStatCategory const, float> > >::insert_unique(const value_type& v) -- STLport 4.6 _tree.c unique insert: descend on key < node key, step back with the instance's iterator operator-- (0x006D9860, out of line) unless at begin(), then _M_insert (0x006DB390) when the predecessor's key is less; pair<iterator, bool> through the hidden pointer
// Leaf: ?insert_unique@?$_Rb_tree@W4EHeroExperienceStatCategory@@U?$pair@$$CBW4EHeroExperienceStatCategory@@M@std@@U?$_Select1st@U?$pair@$$CBW4EHeroExperienceStatCategory@@M@std@@@3@U?$less@W4EHeroExperienceStatCategory@@@3@V?$allocator@U?$pair@$$CBW4EHeroExperienceStatCategory@@M@std@@@3@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@U?$pair@$$CBW4EHeroExperienceStatCategory@@M@std@@U?$_Nonconst_traits@U?$pair@$$CBW4EHeroExperienceStatCategory@@M@std@@@2@@std@@_N@2@ABU?$pair@$$CBW4EHeroExperienceStatCategory@@M@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006DB610, 180 bytes, thiscall `ret 8`; /O2 TU.
// Identity (NF58, round 7): the same tree as its callee _M_insert 0x006DB390 (see that row); its landed caller is CPersistTraits<CHeroStatIncreaseData>::TransferBinaryIn 0x006DC070 (Ego r.pdb CHeroStatIncreaseData::StatIncreases).
// Was the stand-in `_unattributed::InsertUnique_CRetailTree_006db610` (local model); now the explicit instantiation of stlport/fable_map.h's insert_unique (FABLE_RETAIL_STLP_MAP
//   body) with the iterator's operator-- kept out of line (FABLE_RETAIL_STLP_TREE_DEC_OUTOFLINE, as retail calls the per-instance
//   COMDAT 0x006D9860). STLport 4.6.2 source, descriptive-by-library.
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_DEC_OUTOFLINE
#include "stlport/fable_map.h"
#include "engine/EHeroExperienceStatCategory.h"
namespace std {
template pair<_Rb_tree<EHeroExperienceStatCategory, pair<EHeroExperienceStatCategory const, float>, _Select1st<pair<EHeroExperienceStatCategory const, float> >, less<EHeroExperienceStatCategory >, allocator<pair<EHeroExperienceStatCategory const, float> > >::iterator, bool> _Rb_tree<EHeroExperienceStatCategory, pair<EHeroExperienceStatCategory const, float>, _Select1st<pair<EHeroExperienceStatCategory const, float> >, less<EHeroExperienceStatCategory >, allocator<pair<EHeroExperienceStatCategory const, float> > >::insert_unique(pair<EHeroExperienceStatCategory const, float> const&);
}
