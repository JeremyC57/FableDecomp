// std::_Rb_tree<CDefString, pair<CDefString const, long>, _Select1st<pair<CDefString const, long> >, CCompareDefString<long>, allocator<pair<CDefString const, long> > >::_M_insert(x, y, v, w) -- STLport 4.6 _tree.c: create the node (_M_create_node 0x004E597B), link it as y's left or right child (fixing root / leftmost / rightmost), rebalance (_Rb_global<bool>::_Rebalance 0x0042971D), count it, return the iterator
// Leaf: ?_M_insert@?$_Rb_tree@VCDefString@@U?$pair@$$CBVCDefString@@J@std@@U?$_Select1st@U?$pair@$$CBVCDefString@@J@std@@@3@V?$CCompareDefString@J@@V?$allocator@U?$pair@$$CBVCDefString@@J@std@@@3@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@$$CBVCDefString@@J@std@@U?$_Nonconst_traits@U?$pair@$$CBVCDefString@@J@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBVCDefString@@J@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E5BA7, 136 bytes, thiscall `ret 14h` (hidden iterator result + x, y, &v, w);
//   frame-pointer size TU (`optimize("y",off)` + `("s",on)`).
// Identity (NF58, round 7): the map<CDefString, long, CCompareDefString<long>> tree; Ego_r.pdb members of this type:
//   CFireEffectCreationSet::ContainmentMap. Landed callers of its insert_unique 0x004E5B21: 0x004E5A76 _global::GFSerialiseMapBinaryIn.
// Retail facts: signed (`jl`) key compare; _M_create_node 0x004E597B allocates 0x18-byte nodes (`push 18h`) = 0x10 links + the 8-byte
//   value_type.
//   STLport 4.6.2 _tree.c (private member), descriptive-by-library (both PDBs are Dinkumware builds). Was the stand-in
//   `CRetailTree_longkey_buy004e597b::Insert`; now the explicit instantiation of stlport/fable_map.h
//   FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY (the landed 0x0042CB08 pattern).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "stlport/fable_map.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRING_COMPARE_FRIEND
#include "engine/CDefString.h"
template<class T> class CCompareDefString {   // Ego_r.pdb / FableWin.pdb ??R?$CCompareDefString@J@@QBE_NABVCDefString@@0@Z; no shared header
public:
    bool operator()(const CDefString& a, const CDefString& b) const { return a.TablePos < b.TablePos; }
};
namespace std {
template _Rb_tree<CDefString, pair<CDefString const, long>, _Select1st<pair<CDefString const, long> >, CCompareDefString<long>, allocator<pair<CDefString const, long> > >::iterator _Rb_tree<CDefString, pair<CDefString const, long>, _Select1st<pair<CDefString const, long> >, CCompareDefString<long>, allocator<pair<CDefString const, long> > >::_M_insert(_Rb_tree_node_base*, _Rb_tree_node_base*, const pair<CDefString const, long>&, _Rb_tree_node_base*);
}
