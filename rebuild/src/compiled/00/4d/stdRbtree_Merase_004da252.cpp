// std::_Rb_tree<EHeroExperienceStatCategory, pair<const EHeroExperienceStatCategory, float>, ...>::_M_erase(_Rb_tree_node* x) -- STLport 4.6
//   recursive subtree free: recurse on the right child, free the node, walk to the left child
// Leaf: ?_M_erase@?$_Rb_tree@W4EHeroExperienceStatCategory@@U?$pair@$$CBW4EHeroExperienceStatCategory@@M@std@@U?$_Select1st@U?$pair@$$CBW4EHeroExperienceStatCategory@@M@std@@@3@U?$less@W4EHeroExperienceStatCategory@@@3@V?$allocator@U?$pair@$$CBW4EHeroExperienceStatCategory@@M@std@@@3@@std@@AAEXPAU?$_Rb_tree_node@U?$pair@$$CBW4EHeroExperienceStatCategory@@M@std@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DA252, 45 bytes, a COMDAT in a size-optimised TU.
// Identity (NF86, q11107 callee): the _M_erase of CHeroStatIncreaseData::StatIncreases (map<EHeroExperienceStatCategory,float>, Ego_r.pdb): the
//   re-authored ~CPersistTraits<CHeroStatIncreaseData> 0x006DC1F0 binds it so (explicit instantiation, TREE_MERASE_NOINLINE), and its other
//   landed callers destroy CHeroStatIncreaseData values: CHeroExperienceDef::ReadFromStream 0x006DD1C0 / WriteToStream 0x006DC450 (Ego_r
//   CHeroExperienceDef::StatIncreasePerHitType map<EDamageAttribute, CHeroStatIncreaseData>) and the clear 0x004DB853. Byte shape = the
//   EDamageAttribute twin 0x00450903. Descriptive by library (STLport 4.6; both PDBs are Dinkumware builds). Was the stand-in
//   LTextBinTree<LTextGroup*>::LTextTreeWalkThrough::BuildTreeArray (leaf ?BuildTreeArray@@YIXPAUAutoTinyTreeWalker@@UAutoTinyTreeNodeArgument@@@Z;
//   its landed callers bind it under three other stand-ins: NAME_FIXES).
// Port (NF86): one explicit instantiation on stlport/fable_map.h (the 0x00450903 recipe).
#pragma optimize("s",on)
#include "engine/EHeroExperienceStatCategory.h"
#include "stlport/fable_map.h"
namespace std {
template void _Rb_tree<EHeroExperienceStatCategory, pair<const EHeroExperienceStatCategory, float>, _Select1st<pair<const EHeroExperienceStatCategory, float> >, less<EHeroExperienceStatCategory>, allocator<pair<const EHeroExperienceStatCategory, float> > >::_M_erase(_Rb_tree_node<pair<const EHeroExperienceStatCategory, float> >* x);
}
