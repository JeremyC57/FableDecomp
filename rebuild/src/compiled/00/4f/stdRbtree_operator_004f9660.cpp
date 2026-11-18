// std::_Rb_tree<long, pair<long const, CComboMultiplierData>, _Select1st<pair<long const, CComboMultiplierData> >, less<long>, allocator<pair<long const, CComboMultiplierData> > >::operator=(const _Rb_tree&) -- STLport 4.6 _tree.c tree assignment: unless self-assigned, clear 0x004E7720, zero the count, then
//   reset the header to an empty tree or _M_copy 0x004F96D3 the source root under the header, walk leftmost / rightmost, copy the count
// Leaf: ??4?$_Rb_tree@JU?$pair@$$CBJVCComboMultiplierData@@@std@@U?$_Select1st@U?$pair@$$CBJVCComboMultiplierData@@@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJVCComboMultiplierData@@@std@@@2@@std@@QAEAAV01@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004F9660, 115 bytes, size TU (`and dword ptr [..],0`), thiscall `ret 4`.
// NF75 (round 7, land1117fu q10914): an explicit instantiation of stlport/fable_map.h operator= (FABLE_RETAIL_STLP_TREE_ASSIGN_BODY, the NF64 /
//   NF70 recipe of 0x0045C3CF) for the map<long, CComboMultiplierData> tree whose _M_copy 0x004F96D3 and _M_create_node 0x004F9763 NF70
//   re-authored (CHeroExperienceDef::ComboMultiplierMap, Ego_r.pdb). It now binds _M_copy and clear under their STLport spellings. Was the
//   stand-in `CRetailTree_004f96d3::operator=` (bound ?Copy@CRetailTree_004f96d3 / ?Clear@CRetailTree_004f96d3). Descriptive-by-library
//   (retail is STLport 4.6; both PDBs are Dinkumware builds).
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_TREE_ASSIGN_BODY
#include "stlport/fable_map.h"
#include "engine/CComboMultiplierData.h"
namespace std {
template _Rb_tree<long, pair<long const, CComboMultiplierData>, _Select1st<pair<long const, CComboMultiplierData> >, less<long>, allocator<pair<long const, CComboMultiplierData> > >& _Rb_tree<long, pair<long const, CComboMultiplierData>, _Select1st<pair<long const, CComboMultiplierData> >, less<long>, allocator<pair<long const, CComboMultiplierData> > >::operator=(const _Rb_tree<long, pair<long const, CComboMultiplierData>, _Select1st<pair<long const, CComboMultiplierData> >, less<long>, allocator<pair<long const, CComboMultiplierData> > >&);
}
