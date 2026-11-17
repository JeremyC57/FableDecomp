// std::_Rb_tree<int, pair<int const, CCharString>, _Select1st<pair<int const, CCharString> >, less<int >, allocator<pair<int const, CCharString> > >::_M_insert(x, y, v, w) -- STLport 4.6 _tree.c: create the node (_M_create_node 0x00CD2C5B), link it as y's left or right child (fixing root / leftmost / rightmost), rebalance (_Rb_global<bool>::_Rebalance 0x0042971D), count it, return the iterator
// Leaf: ?_M_insert@?$_Rb_tree@HU?$pair@$$CBHVCCharString@@@std@@U?$_Select1st@U?$pair@$$CBHVCCharString@@@std@@@2@U?$less@H@2@V?$allocator@U?$pair@$$CBHVCCharString@@@std@@@2@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@$$CBHVCCharString@@@std@@U?$_Nonconst_traits@U?$pair@$$CBHVCCharString@@@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBHVCCharString@@@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CD300B, 136 bytes, thiscall `ret 14h` (hidden iterator result + x, y, &v, w);
//   frame-pointer size TU (`optimize("y",off)` + `("s",on)`).
// Identity (NF58, round 7): the map<int, CCharString> tree; no Ego_r.pdb member of this exact type; its two sources: both PDB name sets
//   carry the cutscene module's map<int, CCharString> (?PlayCutsceneSound@@YIXAAV?$map@HVCCharString@@...@Z, beside RunCutsceneMacro,
//   whose operator[] 0x00CD3A16 reaches this tree through the forwarder 0x00CD36C2 and the hinted insert_unique 0x00CD34C5), and
//   map<long, CCharString> is already the separate family 0x00472A05 / 0x0047297F (one instance, one address; REJECTED.tsv); node 0x18
//   copies the key and calls the CCharString copy 0x0099EC30 through the pair copy 0x00CD2395. Landed callers of its insert_unique
//   0x00CD3093: 0x00CD34C5 _global::RBTree_TransferAllocateInt. Landed callers of _M_insert besides 0x00CD3093: 0x00CD34C5
//   _global::RBTree_TransferAllocateInt.
// Retail facts: signed (`jl`) key compare; _M_create_node 0x00CD2C5B allocates 0x18-byte nodes (`push 18h`) = 0x10 links + the 8-byte
//   value_type.
//   STLport 4.6.2 _tree.c (private member), descriptive-by-library (both PDBs are Dinkumware builds). Was the stand-in
//   `CRetailTree_longkey_buy00cd2c5b::Insert`; now the explicit instantiation of stlport/fable_map.h
//   FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY (the landed 0x0042CB08 pattern).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "stlport/fable_map.h"
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
namespace std {
template _Rb_tree<int, pair<int const, CCharString>, _Select1st<pair<int const, CCharString> >, less<int >, allocator<pair<int const, CCharString> > >::iterator _Rb_tree<int, pair<int const, CCharString>, _Select1st<pair<int const, CCharString> >, less<int >, allocator<pair<int const, CCharString> > >::_M_insert(_Rb_tree_node_base*, _Rb_tree_node_base*, const pair<int const, CCharString>&, _Rb_tree_node_base*);
}
