// std::_Rb_tree<EWeaponClass, EWeaponClass, _Identity<EWeaponClass >, less<EWeaponClass >, allocator<EWeaponClass > >::_M_insert(x, y, v, w) -- STLport 4.6 _tree.c: create the node (_M_create_node 0x00461CF3), link it as y's left or right child (fixing root / leftmost / rightmost), rebalance (_Rb_global<bool>::_Rebalance 0x0042971D), count it, return the iterator
// Leaf: ?_M_insert@?$_Rb_tree@W4EWeaponClass@@W41@U?$_Identity@W4EWeaponClass@@@std@@U?$less@W4EWeaponClass@@@3@V?$allocator@W4EWeaponClass@@@3@@std@@AAE?AU?$_Rb_tree_iterator@W4EWeaponClass@@U?$_Nonconst_traits@W4EWeaponClass@@@std@@@2@PAU_Rb_tree_node_base@2@0ABW4EWeaponClass@@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00465DBC, 136 bytes, thiscall `ret 14h` (hidden iterator result + x, y, &v, w);
//   frame-pointer size TU (`optimize("y",off)` + `("s",on)`).
// Identity (NF58, round 7): the set<EWeaponClass> tree; Ego_r.pdb members of this type:
//   CCombatAbilityBlockingData::ValidBlockWeaponTypes. Landed callers of its insert_unique 0x00465D36: 0x00465C54
//   CCombatAbilityBlockDefBase::Transfer.
// Retail facts: signed (`jl`) key compare; _M_create_node 0x00461CF3 allocates 0x14-byte nodes (`push 14h`) = 0x10 links + the 4-byte
//   value_type.
//   STLport 4.6.2 _tree.c (private member), descriptive-by-library (both PDBs are Dinkumware builds). Was the stand-in
//   `CRetailTree_longkey_buy00461cf3::Insert`; now the explicit instantiation of stlport/fable_map.h
//   FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY (the landed 0x0042CB08 pattern).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "stlport/fable_map.h"
#include "engine/EWeaponClass.h"
namespace std {
template _Rb_tree<EWeaponClass, EWeaponClass, _Identity<EWeaponClass >, less<EWeaponClass >, allocator<EWeaponClass > >::iterator _Rb_tree<EWeaponClass, EWeaponClass, _Identity<EWeaponClass >, less<EWeaponClass >, allocator<EWeaponClass > >::_M_insert(_Rb_tree_node_base*, _Rb_tree_node_base*, const EWeaponClass&, _Rb_tree_node_base*);
}
