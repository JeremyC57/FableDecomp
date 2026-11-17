// std::_Rb_tree<long, pair<long const, CComboMultiplierData>, ...>::_M_copy(_Link_type x, _Link_type p) -- STLport 4.6 _tree.c structural
//   copy: clone the root (_M_create_node 0x004F9763), recurse right, then walk left cloning each node and recursing into its right subtree
// Leaf: ?_M_copy@?$_Rb_tree@JU?$pair@$$CBJVCComboMultiplierData@@@std@@U?$_Select1st@U?$pair@$$CBJVCComboMultiplierData@@@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJVCComboMultiplierData@@@std@@@2@@std@@AAEPAU?$_Rb_tree_node@U?$pair@$$CBJVCComboMultiplierData@@@std@@@2@PAU32@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004F96D3, size TU.
// NF70 (round 7, land1117fu q10740): an explicit instantiation of stlport/fable_map.h's _M_copy (FABLE_RETAIL_STLP_TREE_MCOPY_BODY, NF61's
//   recipe of the 0x0045C442 family) for the map<long, CComboMultiplierData> tree; it now binds _M_create_node 0x004F9763 under the STLport
//   spelling (was the stand-in CRetailTree_004f9763::Copy / Buynode). Descriptive-by-library (STLport 4.6).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_TREE_MCOPY_BODY
#define FABLE_RETAIL_STLP_MAP_COPY_OUTOFLINE
#include "stlport/fable_map.h"
#include "engine/CComboMultiplierData.h"
namespace std {
template _Rb_tree_node<pair<long const, CComboMultiplierData> >* _Rb_tree<long, pair<long const, CComboMultiplierData>, _Select1st<pair<long const, CComboMultiplierData> >, less<long>, allocator<pair<long const, CComboMultiplierData> > >::_M_copy(_Rb_tree_node<pair<long const, CComboMultiplierData> >*, _Rb_tree_node<pair<long const, CComboMultiplierData> >*);
}
