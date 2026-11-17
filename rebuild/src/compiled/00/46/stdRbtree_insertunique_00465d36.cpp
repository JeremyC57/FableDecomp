// std::_Rb_tree<EWeaponClass, EWeaponClass, _Identity<EWeaponClass >, less<EWeaponClass >, allocator<EWeaponClass > >::insert_unique(const value_type& v) -- STLport 4.6 _tree.c unique insert: descend on key < node key, step back with _Rb_global<bool>::_M_decrement (0x004294E2) unless at begin(), then _M_insert (0x00465DBC) when the predecessor's key is less; pair<iterator, bool> through the hidden pointer
// Leaf: ?insert_unique@?$_Rb_tree@W4EWeaponClass@@W41@U?$_Identity@W4EWeaponClass@@@std@@U?$less@W4EWeaponClass@@@3@V?$allocator@W4EWeaponClass@@@3@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@W4EWeaponClass@@U?$_Nonconst_traits@W4EWeaponClass@@@std@@@std@@_N@2@ABW4EWeaponClass@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00465D36, 134 bytes, thiscall `ret 8`; frame-pointer size TU.
// Identity (NF58, round 7): the set<EWeaponClass> tree; Ego_r.pdb members of this type:
//   CCombatAbilityBlockingData::ValidBlockWeaponTypes. Landed callers of its insert_unique 0x00465D36: 0x00465C54
//   CCombatAbilityBlockDefBase::Transfer.
//   Its callee _M_insert 0x00465DBC is the same instance. STLport 4.6.2 source, descriptive-by-library. Was the stand-in
//   `CRetailTree_longkey_ins00465dbc::insert`; now an explicit instantiation over stlport/fable_map.h (FABLE_RETAIL_STLP_MAP body;
//   FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ: out-of-line _M_decrement), the landed 0x0042CEE3 pattern.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ
#include "stlport/fable_map.h"
#include "engine/EWeaponClass.h"
namespace std {
template pair<_Rb_tree<EWeaponClass, EWeaponClass, _Identity<EWeaponClass >, less<EWeaponClass >, allocator<EWeaponClass > >::iterator, bool> _Rb_tree<EWeaponClass, EWeaponClass, _Identity<EWeaponClass >, less<EWeaponClass >, allocator<EWeaponClass > >::insert_unique(const EWeaponClass&);
}
