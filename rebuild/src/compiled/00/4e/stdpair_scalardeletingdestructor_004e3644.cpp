// std::pair<const EDamageAttribute, CHeroStatIncreaseData>::`scalar deleting destructor'(unsigned int flags) -- destroy the
// CHeroStatIncreaseData value (+4: its StatIncreases map<EHeroExperienceStatCategory, float>), free the pair when bit 0 is set
// Leaf: ??_G?$pair@$$CBW4EDamageAttribute@@VCHeroStatIncreaseData@@@std@@QAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E3644, 31 bytes, thiscall `ret 4`; size-optimised (`pop ecx` after the delete).
// Identity (NF90, q11178 binder): the node value of the map<EDamageAttribute, CHeroStatIncreaseData> tree (Ego_r.pdb
//   CHeroExperienceDef::StatIncreasePerHitType). That tree's _M_erase 0x004E6E43 calls it with flag 0 (`push 0 ; lea ecx,[esi+10h]`:
//   STLport erase runs `_Destroy(&node->_M_value_field)`, which VC7.1 under optimize("s") compiles as `push 0 ; call ??_G`, the 0x005799DE
//   recipe). Its +4 callee 0x004DD1DB is the ~_Rb_tree of map<EHeroExperienceStatCategory, float> (NF88), CHeroStatIncreaseData's only
//   member. Byte twin of 0x004E3625 (NF83, pair<const long, CComboMultiplierData>). Descriptive-by-library (STLport pair; both PDBs are
//   Dinkumware builds; Ego_r names the Dinkumware sibling ??_G?$pair@$$CBW4EObjectAugmentationType@@... at 0x00504923). Was the propagated
//   label CLandscapeBackgroundPatch::`vector_deleting_destructor' (catalog _global::ScalarDeletingDtor_004e3644, a free __fastcall model
//   binding 0x004DD1DB as ?Sub_Dtor_004dd1db@@YIXPAX@Z).
// Port (NF90): one explicit instantiation of the STLport _Destroy on stlport/fable_map.h + engine/CHeroStatIncreaseData.h (the 0x004E3625 form).
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_TREE_CLEAR_OUTOFLINE
#include "stlport/fable_map.h"
#include "engine/EDamageAttribute.h"
#include "engine/CHeroStatIncreaseData.h"
namespace std {
template void _Destroy(pair<const EDamageAttribute, CHeroStatIncreaseData>* __pointer);
}
