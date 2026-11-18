// std::_Rb_tree<CDefPointer<const CArmourDef>, pair<CDefPointer<const CArmourDef> const, float>, _Select1st<pair<CDefPointer<const CArmourDef> const, float> >, CCompareDefPointer<const CArmourDef, float >, allocator<pair<CDefPointer<const CArmourDef> const, float> > >::_M_insert(x, y, v, w) -- STLport 4.6 _tree.c: allocate and copy-construct the node (_M_create_node, inline here), link it as y's left or right child (fixing root / leftmost / rightmost), rebalance (_Rb_global<bool>::_Rebalance 0x0042971D), count it, return the iterator
// Leaf: ?_M_insert@?$_Rb_tree@V?$CDefPointer@$$CBVCArmourDef@@@@U?$pair@$$CBV?$CDefPointer@$$CBVCArmourDef@@@@M@std@@U?$_Select1st@U?$pair@$$CBV?$CDefPointer@$$CBVCArmourDef@@@@M@std@@@3@V?$CCompareDefPointer@$$CBVCArmourDef@@M@@V?$allocator@U?$pair@$$CBV?$CDefPointer@$$CBVCArmourDef@@@@M@std@@@3@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@$$CBV?$CDefPointer@$$CBVCArmourDef@@@@M@std@@U?$_Nonconst_traits@U?$pair@$$CBV?$CDefPointer@$$CBVCArmourDef@@@@M@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBV?$CDefPointer@$$CBVCArmourDef@@@@M@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00869CA0, 205 bytes, thiscall `ret 14h` (hidden iterator result + x, y, &v, w); /O2 TU.
// Identity (NF58, round 7): the map<CDefPointer<const CArmourDef>, float> tree; no Ego_r.pdb member of this exact type. Landed callers
//   of this address: 0x00868E50 _global::GetArmourWeightsFromThing; 0x00869020 CHitParameters::SetAsModifiedByArmour.
//   STLport 4.6.2 _tree.c (private member), descriptive-by-library (both PDBs are Dinkumware builds). Was the stand-in
//   `std::multimap::_M_insert`; now the explicit instantiation of stlport/fable_map.h
//   FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY (NF56's 0x0069D520 pattern; malloc 0x00BFEA0E is the STLport node allocator).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "stlport/fable_map.h"
#include "engine/CCompareDefPointer.h"
#include "engine/CArmourDef.h"
namespace std {
template _Rb_tree<CDefPointer<const CArmourDef>, pair<CDefPointer<const CArmourDef> const, float>, _Select1st<pair<CDefPointer<const CArmourDef> const, float> >, CCompareDefPointer<const CArmourDef, float >, allocator<pair<CDefPointer<const CArmourDef> const, float> > >::iterator _Rb_tree<CDefPointer<const CArmourDef>, pair<CDefPointer<const CArmourDef> const, float>, _Select1st<pair<CDefPointer<const CArmourDef> const, float> >, CCompareDefPointer<const CArmourDef, float >, allocator<pair<CDefPointer<const CArmourDef> const, float> > >::_M_insert(_Rb_tree_node_base*, _Rb_tree_node_base*, const pair<CDefPointer<const CArmourDef> const, float>&, _Rb_tree_node_base*);
}
