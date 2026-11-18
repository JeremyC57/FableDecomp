// std::map<EHeroExperienceStatCategory, float>::map(const map& x) -- STLport map copy constructor (the tree copy constructor expanded):
// allocate the header node, then either mark the tree empty or clone the other tree's nodes (_M_copy) and set leftmost / rightmost,
// finally take the node count
// Leaf: ??0?$map@W4EHeroExperienceStatCategory@@MU?$less@W4EHeroExperienceStatCategory@@@std@@V?$allocator@U?$pair@$$CBW4EHeroExperienceStatCategory@@M@std@@@3@@std@@QAE@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004F9960, length 126 bytes (ret 4), a COMDAT in a size-optimised TU.
// Identity (NF90, q11180): the map<EHeroExperienceStatCategory, float> (Ego_r.pdb CHeroStatIncreaseData::StatIncreases) copy constructor
//   (CHeroStatIncreaseData's implicit copy): the landed 0x004F9934 (_M_create_node of the map<EDamageAttribute, CHeroStatIncreaseData>
//   tree), 0x006DCBF0 (_M_insert of that tree) and the ported 0x006DD1C0 (NF88, MAP_COPY_OUTOFLINE) all bind it under this spelling.
//   Byte twin of 0x004F97B3 (NF78, map<EDamageAttribute, float>). Its _M_copy callee is 0x004F99DE. Descriptive-by-library: STLport
//   instance, no PDB holds it (both PDBs are Dinkumware builds). Was the stand-in _Rb_tree::_Rb_tree (leaf ??0_Rb_tree@@QAE@ABV0@@Z, a
//   local SRbNode model).
// Port (NF90): stlport/fable_map.h with FABLE_RETAIL_STLP_TREE_COPY_CTOR_BODY and FABLE_RETAIL_STLP_MAP_COPY_CTOR_BODY; one explicit
//   instantiation (the 0x004F97B3 recipe).
#include "fable_compiler.h"
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_TREE_COPY_CTOR_BODY
#define FABLE_RETAIL_STLP_MAP_COPY_CTOR_BODY
#include "engine/EHeroExperienceStatCategory.h"
#include "stlport/fable_map.h"

template std::map<EHeroExperienceStatCategory, float>::map(const std::map<EHeroExperienceStatCategory, float>&);
