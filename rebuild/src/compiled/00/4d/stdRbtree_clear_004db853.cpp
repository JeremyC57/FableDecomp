// std::_Rb_tree<EHeroExperienceStatCategory, pair<const EHeroExperienceStatCategory, float>, ...>::clear() -- STLport 4.6: if the tree is not
//   empty, _M_erase(_M_root()) (0x004DA252), then reset the header (leftmost = header, root = 0, rightmost = header) and the node count
// Leaf: ?clear@?$_Rb_tree@W4EHeroExperienceStatCategory@@U?$pair@$$CBW4EHeroExperienceStatCategory@@M@std@@U?$_Select1st@U?$pair@$$CBW4EHeroExperienceStatCategory@@M@std@@@3@U?$less@W4EHeroExperienceStatCategory@@@3@V?$allocator@U?$pair@$$CBW4EHeroExperienceStatCategory@@M@std@@@3@@std@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DB853, 41 bytes, a COMDAT in a size-optimised TU.
// Identity (NF88, q11152): the clear of the map<EHeroExperienceStatCategory, float> tree (CHeroStatIncreaseData::StatIncreases, Ego_r.pdb).
//   Its one callee is that tree's _M_erase 0x004DA252 (NF86, RENAMES s1); a template member's clear calls its own instantiation's _M_erase.
//   Byte shape = the EObjectAugmentationType twin 0x00451240 and the EDamageAttribute twin 0x0045128D. Descriptive by library (STLport 4.6;
//   both PDBs are Dinkumware builds). Was the stand-in CTCCarryable::OnKill (leaf ?OnKill@T@@QAEXXZ over a local node model; _M_erase bound
//   as ?BuildTreeArray_004da252@T@@QAEXPAX@Z).
// Port (NF88): one explicit instantiation on stlport/fable_map.h (the 0x00451240 recipe).
#pragma optimize("s", on)
#include "engine/EHeroExperienceStatCategory.h"
#include "stlport/fable_map.h"
namespace std {
template void _Rb_tree<EHeroExperienceStatCategory, pair<const EHeroExperienceStatCategory, float>, _Select1st<pair<const EHeroExperienceStatCategory, float> >, less<EHeroExperienceStatCategory>, allocator<pair<const EHeroExperienceStatCategory, float> > >::clear();
}
