// std::_Rb_tree<EClothingSuitPart, pair<EClothingSuitPart const, bool>, _Select1st<pair<EClothingSuitPart const, bool> >, less<EClothingSuitPart >, allocator<pair<EClothingSuitPart const, bool> > >::_M_insert(x, y, v, w) -- STLport 4.6 _tree.c: create the node (_M_create_node 0x005BAF06), link it as y's left or right child (fixing root / leftmost / rightmost), rebalance (_Rb_global<bool>::_Rebalance 0x0042971D), count it, return the iterator
// Leaf: ?_M_insert@?$_Rb_tree@W4EClothingSuitPart@@U?$pair@$$CBW4EClothingSuitPart@@_N@std@@U?$_Select1st@U?$pair@$$CBW4EClothingSuitPart@@_N@std@@@3@U?$less@W4EClothingSuitPart@@@3@V?$allocator@U?$pair@$$CBW4EClothingSuitPart@@_N@std@@@3@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@$$CBW4EClothingSuitPart@@_N@std@@U?$_Nonconst_traits@U?$pair@$$CBW4EClothingSuitPart@@_N@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBW4EClothingSuitPart@@_N@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BB220, 136 bytes, thiscall `ret 14h` (hidden iterator result + x, y, &v, w);
//   frame-pointer size TU (`optimize("s", on)`).
// Identity (NF58, round 7): the map<EClothingSuitPart, bool> tree; no Ego_r.pdb member of this exact type; its two sources: the landed
//   CTCInventoryClothing::ConstructScreen 0x005B76D1 / ConstructScreenPC 0x005B8743 call this map<EClothingSuitPart, bool>'s operator[]
//   0x005BBC4E -> forwarder 0x005BBB7D (bound ?insert@?$map@W4EClothingSuitPart@@_N...) -> hinted insert_unique 0x005BB85A (binds this
//   spelling), and the node creator 0x005BAF06 copies the key dword and a single byte at +4 (the bool). Landed callers of its
//   insert_unique 0x005BB2A8: 0x005BB85A _global::CMemoryAllocatorVariableSize_InsertOrFind_24Byte. Landed callers of _M_insert besides
//   0x005BB2A8: 0x005BB85A _global::CMemoryAllocatorVariableSize_InsertOrFind_24Byte.
// Retail facts: signed (`jl`) key compare; _M_create_node 0x005BAF06 allocates 0x18-byte nodes (`push 18h`) = 0x10 links + the 8-byte
//   value_type.
//   STLport 4.6.2 _tree.c (private member), descriptive-by-library (both PDBs are Dinkumware builds). Was the stand-in
//   `CRetailTree_longkey_buy005baf06::Insert`; now the explicit instantiation of stlport/fable_map.h
//   FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY (the landed 0x0042CB08 pattern).
// NF419 (2026-11-25): no `#pragma optimize("y", off)`: the leaf is byte- and relocation-identical without it (size mode keeps
//   its frame); it only framed end-of-TU template and compiler-generated copies (each copy checked in the T3 nf416 audit).
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "stlport/fable_map.h"
#include "engine/EClothingSuitPart.h"
namespace std {
template _Rb_tree<EClothingSuitPart, pair<EClothingSuitPart const, bool>, _Select1st<pair<EClothingSuitPart const, bool> >, less<EClothingSuitPart >, allocator<pair<EClothingSuitPart const, bool> > >::iterator _Rb_tree<EClothingSuitPart, pair<EClothingSuitPart const, bool>, _Select1st<pair<EClothingSuitPart const, bool> >, less<EClothingSuitPart >, allocator<pair<EClothingSuitPart const, bool> > >::_M_insert(_Rb_tree_node_base*, _Rb_tree_node_base*, const pair<EClothingSuitPart const, bool>&, _Rb_tree_node_base*);
}
