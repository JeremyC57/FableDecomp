// std::_Rb_tree<long, pair<const long, CComboMultiplierData>, ...>::_M_insert -- map<long, CComboMultiplierData> node insertion
// Leaf: ?_M_insert@?$_Rb_tree@JU?$pair@$$CBJVCComboMultiplierData@@@std@@U?$_Select1st@U?$pair@$$CBJVCComboMultiplierData@@@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJVCComboMultiplierData@@@std@@@2@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@$$CBJVCComboMultiplierData@@@std@@U?$_Nonconst_traits@U?$pair@$$CBJVCComboMultiplierData@@@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBJVCComboMultiplierData@@@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006DCB20, 198 bytes, thiscall, `ret 14h` (hidden iterator + x, y, value, w). The STLport 4.6 _tree.c
//   _M_insert of map<long, CComboMultiplierData> (Ego_r.pdb CHeroExperienceDef::ComboMultiplierMap): _M_create_node is expanded in the
//   left arm (malloc 0x00BFEA0E + the pair copy with the inner map<EDamageAttribute,float> copy constructor 0x004F97B3) and called out of
//   line in the right arm (0x004F9763); _Rb_global<bool>::_Rebalance 0x0042971D.
// NF62 (2026-11-17): re-authored as the explicit instantiation on stlport/fable_map.h (ORIGINAL_INSERT_BODY + MAP_COPY_OUTOFLINE, the
//   inner map copy kept a call) and engine/CComboMultiplierData.h; the CUIBinaryTree / CInnerMap / SUITreeValue / SUITreePair stand-ins
//   deleted. Was landed as ?Insert@CUIBinaryTree@@QAE?AUiterator@1@PAUnode@1@0ABUSUITreePair@@J@Z.
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#define FABLE_RETAIL_STLP_MAP_COPY_OUTOFLINE
#include "stlport/fable_map.h"
#include "stlport/fable_containers.h"
#include "engine/CComboMultiplierData.h"
namespace std {
template _Rb_tree<long, pair<long const, CComboMultiplierData>, _Select1st<pair<long const, CComboMultiplierData> >, less<long>, allocator<pair<long const, CComboMultiplierData> > >::iterator _Rb_tree<long, pair<long const, CComboMultiplierData>, _Select1st<pair<long const, CComboMultiplierData> >, less<long>, allocator<pair<long const, CComboMultiplierData> > >::_M_insert(_Rb_tree_node_base*, _Rb_tree_node_base*, const pair<long const, CComboMultiplierData>&, _Rb_tree_node_base*);
}
