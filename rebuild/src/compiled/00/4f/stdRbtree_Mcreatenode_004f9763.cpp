// std::_Rb_tree<long, pair<long const, CComboMultiplierData>, ...>::_M_create_node(const value_type& x) -- STLport 4.6 _tree.h: allocate a node
//   (malloc 0x28) and copy-construct the value into it (pair<const long, CComboMultiplierData> copy constructor 0x004F9784, out of line here)
// Leaf: ?_M_create_node@?$_Rb_tree@JU?$pair@$$CBJVCComboMultiplierData@@@std@@U?$_Select1st@U?$pair@$$CBJVCComboMultiplierData@@@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJVCComboMultiplierData@@@std@@@2@@std@@IAEPAU?$_Rb_tree_node@U?$pair@$$CBJVCComboMultiplierData@@@std@@@2@ABU?$pair@$$CBJVCComboMultiplierData@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004F9763, size TU.
// NF70 (round 7, land1117fu q10740): an explicit instantiation of stlport/fable_map.h's _M_create_node for the map<long, CComboMultiplierData>
//   tree whose _M_insert 0x006DCB20 (NF62) calls it; was the stand-in CRetailList_node0x28::Buynode. Descriptive-by-library (STLport 4.6).
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_MAP_COPY_OUTOFLINE
#include "stlport/fable_map.h"
#include "engine/CComboMultiplierData.h"
namespace std {
template _Rb_tree<long, pair<long const, CComboMultiplierData>, _Select1st<pair<long const, CComboMultiplierData> >, less<long>, allocator<pair<long const, CComboMultiplierData> > >::_Link_type _Rb_tree<long, pair<long const, CComboMultiplierData>, _Select1st<pair<long const, CComboMultiplierData> >, less<long>, allocator<pair<long const, CComboMultiplierData> > >::_M_create_node(const pair<long const, CComboMultiplierData>&);
}
