// std::_Rb_tree<EHeroExperienceStatCategory, pair<const EHeroExperienceStatCategory, float>, ...>::~_Rb_tree() -- STLport 4.6 tree destructor:
//   clear() (the out-of-line clear 0x004DB853), then _Rb_tree_base frees the header node
// Leaf: ??1?$_Rb_tree@W4EHeroExperienceStatCategory@@U?$pair@$$CBW4EHeroExperienceStatCategory@@M@std@@U?$_Select1st@U?$pair@$$CBW4EHeroExperienceStatCategory@@M@std@@@3@U?$less@W4EHeroExperienceStatCategory@@@3@V?$allocator@U?$pair@$$CBW4EHeroExperienceStatCategory@@M@std@@@3@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DD1DB, 23 bytes, a COMDAT in a size-optimised TU (`pop ecx` after free).
// Identity (NF88, q11152 follow-on): the destructor of the map<EHeroExperienceStatCategory, float> tree (CHeroStatIncreaseData::StatIncreases,
//   Ego_r.pdb). Its one call is that tree's clear 0x004DB853 (NF88, whose callee is the tree's _M_erase 0x004DA252, NF86). Byte shape = the
//   EObjectAugmentationType twin 0x00451A85 (NF86) and the EDamageAttribute twin 0x00451AC6 (NF83). Descriptive by library (STLport; both PDBs
//   are Dinkumware builds). Was the stand-in _global::StdList_Dtor_004dd1db (leaf ?OnReadFinished_4@@YIXPAUCActiveFile@@@Z over a local
//   CActiveFile model, clear bound as ?helper_4db853@@YIXPAUCActiveFile@@@Z).
// Port (NF88): one explicit instantiation on stlport/fable_map.h with FABLE_RETAIL_STLP_TREE_CLEAR_OUTOFLINE (the 0x00451A85 recipe).
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_TREE_CLEAR_OUTOFLINE
#include "engine/EHeroExperienceStatCategory.h"
#include "stlport/fable_map.h"

template std::_Rb_tree<EHeroExperienceStatCategory, std::pair<const EHeroExperienceStatCategory, float>, std::_Select1st<std::pair<const EHeroExperienceStatCategory, float> >, std::less<EHeroExperienceStatCategory>, std::allocator<std::pair<const EHeroExperienceStatCategory, float> > >::~_Rb_tree();
