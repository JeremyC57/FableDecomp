// std::pair<const long, CComboMultiplierData>::`scalar deleting destructor'(unsigned int flags) -- destroy the CComboMultiplierData value
// (+4: its ComboMultiplier map<EDamageAttribute, float>), free the pair when bit 0 is set
// Leaf: ??_G?$pair@$$CBJVCComboMultiplierData@@@std@@QAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E3625, 31 bytes, thiscall `ret 4`; size-optimised (`pop ecx` after the delete).
// Identity (NF83, round 7, q11010): the node value of the map<long, CComboMultiplierData> tree (Ego_r.pdb
//   CHeroExperienceDef::ComboMultiplierMap). That tree's _M_erase 0x004E6E0C (NF81, RM) calls it with flag 0: STLport erase runs
//   `_Destroy(&node->_M_value_field)`, which VC7.1 under optimize("s") compiles as `push 0 ; call ??_G` (the 0x005799DE recipe).
//   Its +4 callee 0x00451AC6 is the inner EDamageAttribute map's destructor (clear 0x0045128D, then the header free).
//   Descriptive-by-library (STLport pair; both PDBs are Dinkumware builds). Was the propagated label
//   CLandscapeBackgroundPatch::`vector_deleting_destructor' (catalog _global::ScalarDeletingDtor_004e3625, a free __fastcall model).
// Port (NF83): one explicit instantiation of the STLport _Destroy on stlport/fable_map.h + engine/CComboMultiplierData.h.
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_TREE_CLEAR_OUTOFLINE
#include "stlport/fable_map.h"
#include "engine/CComboMultiplierData.h"
namespace std {
template void _Destroy(pair<const long, CComboMultiplierData>* __pointer);
}
