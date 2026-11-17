// std::_Rb_tree<EDamageAttribute, pair<const EDamageAttribute, CHeroStatIncreaseData>, ...>::_M_create_node(const value_type& x) -- STLport
//   4.6 _tree.h: allocate a node and copy-construct the pair into it
// Flags: /d2inlS22
// Inline-budget: /d2inlS22 (EXTRA_RULES 2026-11-08 preference 2, source untouched) -- retail expands _Construct and pair<const EDamageAttribute, CHeroStatIncreaseData>'s copy constructor here (key word, then the out-of-line map copy 0x004F9960 on +4); at the default /Os budget VC7.1 calls the pair copy as a COMDAT (33 v 44 bytes). Sweep: /d2inlS15 DIFFER 33v44, /d2inlS22 / 30 / 60 / 200 RELOCATION_MATCH (16..21 not tried).
// Leaf: ?_M_create_node@?$_Rb_tree@W4EDamageAttribute@@U?$pair@$$CBW4EDamageAttribute@@VCHeroStatIncreaseData@@@std@@U?$_Select1st@U?$pair@$$CBW4EDamageAttribute@@VCHeroStatIncreaseData@@@std@@@3@U?$less@W4EDamageAttribute@@@3@V?$allocator@U?$pair@$$CBW4EDamageAttribute@@VCHeroStatIncreaseData@@@std@@@3@@std@@IAEPAU?$_Rb_tree_node@U?$pair@$$CBW4EDamageAttribute@@VCHeroStatIncreaseData@@@std@@@2@ABU?$pair@$$CBW4EDamageAttribute@@VCHeroStatIncreaseData@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004F9934, 44 bytes, thiscall (this unused), `ret 4` (size-optimised module).
// Identity (proven, landed): the value copy is the map<EHeroExperienceStatCategory, float> copy constructor 0x004F9960 (NF90, q11180), i.e.
//   the implicit copy of Ego_r.pdb CHeroStatIncreaseData { StatIncreases }; the tree copy 0x004F98A4 (NF90) is reached from
//   CHeroExperienceDef::operator= 0x004F914A: Ego_r.pdb StatIncreasePerHitType is map<EDamageAttribute, CHeroStatIncreaseData>.
// Access (NF90, q11180 follow-on): STLport declares _M_create_node protected (IAE), as the byte-twin row 0x004F9763 (NF70) and 64 landed
//   bindings spell it; the old all-local model made it public (QAE), which no caller bound. Byte twin of 0x004F9763.
// Port (NF90): an explicit instantiation of stlport/fable_map.h's _M_create_node (the 0x004F9763 recipe; MAP_COPY_OUTOFLINE keeps the
//   value's map copy constructor a call). Grades 1-5 tried header opt-ins forcing the pair copy / _Construct inline (RM, withdrawn for the
//   less invasive per-row flag above).
//   Was a local allocator / pair / map / _Rb_tree model.
#include "fable_compiler.h"
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_MAP_COPY_OUTOFLINE
#include "stlport/fable_map.h"
#include "engine/EDamageAttribute.h"
#include "engine/CHeroStatIncreaseData.h"
namespace std {
template _Rb_tree<EDamageAttribute, pair<const EDamageAttribute, CHeroStatIncreaseData>, _Select1st<pair<const EDamageAttribute, CHeroStatIncreaseData> >, less<EDamageAttribute>, allocator<pair<const EDamageAttribute, CHeroStatIncreaseData> > >::_Link_type _Rb_tree<EDamageAttribute, pair<const EDamageAttribute, CHeroStatIncreaseData>, _Select1st<pair<const EDamageAttribute, CHeroStatIncreaseData> >, less<EDamageAttribute>, allocator<pair<const EDamageAttribute, CHeroStatIncreaseData> > >::_M_create_node(const pair<const EDamageAttribute, CHeroStatIncreaseData>&);
}
