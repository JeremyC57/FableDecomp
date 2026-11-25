// std::_Rb_tree<long, pair<long const, CEnginePrimitiveHandle>, _Select1st<pair<long const, CEnginePrimitiveHandle> >, less<long >, allocator<pair<long const, CEnginePrimitiveHandle> > >::_M_insert(x, y, v, w) -- STLport 4.6 _tree.c: create the node (_M_create_node 0x005BBBA5), link it as y's left or right child (fixing root / leftmost / rightmost), rebalance (_Rb_global<bool>::_Rebalance 0x0042971D), count it, return the iterator
// Leaf: ?_M_insert@?$_Rb_tree@JU?$pair@$$CBJVCEnginePrimitiveHandle@@@std@@U?$_Select1st@U?$pair@$$CBJVCEnginePrimitiveHandle@@@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJVCEnginePrimitiveHandle@@@std@@@2@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@$$CBJVCEnginePrimitiveHandle@@@std@@U?$_Nonconst_traits@U?$pair@$$CBJVCEnginePrimitiveHandle@@@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBJVCEnginePrimitiveHandle@@@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BBCBE, 136 bytes, thiscall `ret 14h` (hidden iterator result + x, y, &v, w);
//   frame-pointer size TU (`optimize("s", on)`).
// Identity (NF58, round 7): the map<long, CEnginePrimitiveHandle> tree; Ego_r.pdb members of this type:
//   CHeroPostcardGenerator::PrimitiveHandles, CTCInventoryClothing::MorphPrimitiveHandles. Landed callers of its insert_unique
//   0x005BBD46: 0x005BBE32 _global::CMemoryAllocatorVariableSize_InsertOrFind_ScriptRight. Landed callers of _M_insert besides
//   0x005BBD46: 0x005BBE32 _global::CMemoryAllocatorVariableSize_InsertOrFind_ScriptRight.
// Retail facts: signed (`jl`) key compare; _M_create_node 0x005BBBA5 allocates 0x1C-byte nodes (`push 1Ch`) = 0x10 links + the 12-byte
//   value_type.
//   STLport 4.6.2 _tree.c (private member), descriptive-by-library (both PDBs are Dinkumware builds). Was the stand-in
//   `CRetailTree_longkey_buy005bbba5::Insert`; now the explicit instantiation of stlport/fable_map.h
//   FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY (the landed 0x0042CB08 pattern).
// NF419 (2026-11-25): no `#pragma optimize("y", off)`: the leaf is byte- and relocation-identical without it (size mode keeps
//   its frame); it only framed end-of-TU template and compiler-generated copies (each copy checked in the T3 nf416 audit).
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "stlport/fable_map.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#include "engine/CEnginePrimitiveHandle.h"
namespace std {
template _Rb_tree<long, pair<long const, CEnginePrimitiveHandle>, _Select1st<pair<long const, CEnginePrimitiveHandle> >, less<long >, allocator<pair<long const, CEnginePrimitiveHandle> > >::iterator _Rb_tree<long, pair<long const, CEnginePrimitiveHandle>, _Select1st<pair<long const, CEnginePrimitiveHandle> >, less<long >, allocator<pair<long const, CEnginePrimitiveHandle> > >::_M_insert(_Rb_tree_node_base*, _Rb_tree_node_base*, const pair<long const, CEnginePrimitiveHandle>&, _Rb_tree_node_base*);
}
