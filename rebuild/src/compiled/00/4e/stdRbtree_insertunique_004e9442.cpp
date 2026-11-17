// std::_Rb_tree<EObjectAugmentationType, pair<EObjectAugmentationType const, long>, _Select1st<pair<EObjectAugmentationType const, long> >, less<EObjectAugmentationType >, allocator<pair<EObjectAugmentationType const, long> > >::insert_unique(const value_type& v) -- STLport 4.6 _tree.c unique insert: descend on key < node key, step back with _Rb_global<bool>::_M_decrement (0x004294E2) unless at begin(), then _M_insert (0x004E94C8) when the predecessor's key is less; pair<iterator, bool> through the hidden pointer
// Leaf: ?insert_unique@?$_Rb_tree@W4EObjectAugmentationType@@U?$pair@$$CBW4EObjectAugmentationType@@J@std@@U?$_Select1st@U?$pair@$$CBW4EObjectAugmentationType@@J@std@@@3@U?$less@W4EObjectAugmentationType@@@3@V?$allocator@U?$pair@$$CBW4EObjectAugmentationType@@J@std@@@3@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@U?$pair@$$CBW4EObjectAugmentationType@@J@std@@U?$_Nonconst_traits@U?$pair@$$CBW4EObjectAugmentationType@@J@std@@@2@@std@@_N@2@ABU?$pair@$$CBW4EObjectAugmentationType@@J@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E9442, 134 bytes, thiscall `ret 8`; frame-pointer size TU.
// Identity (NF58, round 7): the map<EObjectAugmentationType, long> tree; Ego_r.pdb members of this type: CWeaponDef::ArrowTrails.
//   Landed callers of its insert_unique 0x004E9442: 0x004E9393 GFSerialiseMapBinaryIn<long, EObjectAugmentationType, std::less<EObjec;
//   0x005C4F42 CTCInventoryWeapons::ConstructAugmentationsLeftDescription.
//   Its callee _M_insert 0x004E94C8 is the same instance. STLport 4.6.2 source, descriptive-by-library. Was the stand-in
//   `CRetailTree_longkey_ins004e94c8::insert`; now an explicit instantiation over stlport/fable_map.h (FABLE_RETAIL_STLP_MAP body;
//   FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ: out-of-line _M_decrement), the landed 0x0042CEE3 pattern.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ
#include "stlport/fable_map.h"
#include "engine/EObjectAugmentationType.h"
namespace std {
template pair<_Rb_tree<EObjectAugmentationType, pair<EObjectAugmentationType const, long>, _Select1st<pair<EObjectAugmentationType const, long> >, less<EObjectAugmentationType >, allocator<pair<EObjectAugmentationType const, long> > >::iterator, bool> _Rb_tree<EObjectAugmentationType, pair<EObjectAugmentationType const, long>, _Select1st<pair<EObjectAugmentationType const, long> >, less<EObjectAugmentationType >, allocator<pair<EObjectAugmentationType const, long> > >::insert_unique(const pair<EObjectAugmentationType const, long>&);
}
