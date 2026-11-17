// std::_Rb_tree<EHeroTrainableStatType, pair<EHeroTrainableStatType const, long>, _Select1st<pair<EHeroTrainableStatType const, long> >, less<EHeroTrainableStatType >, allocator<pair<EHeroTrainableStatType const, long> > >::insert_unique(const value_type& v) -- STLport 4.6 _tree.c unique insert: descend on key < node key, step back with _Rb_global<bool>::_M_decrement (0x004294E2) unless at begin(), then _M_insert (0x00463EB4) when the predecessor's key is less; pair<iterator, bool> through the hidden pointer
// Leaf: ?insert_unique@?$_Rb_tree@W4EHeroTrainableStatType@@U?$pair@$$CBW4EHeroTrainableStatType@@J@std@@U?$_Select1st@U?$pair@$$CBW4EHeroTrainableStatType@@J@std@@@3@U?$less@W4EHeroTrainableStatType@@@3@V?$allocator@U?$pair@$$CBW4EHeroTrainableStatType@@J@std@@@3@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@U?$pair@$$CBW4EHeroTrainableStatType@@J@std@@U?$_Nonconst_traits@U?$pair@$$CBW4EHeroTrainableStatType@@J@std@@@2@@std@@_N@2@ABU?$pair@$$CBW4EHeroTrainableStatType@@J@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00463E2E, 134 bytes, thiscall `ret 8`; frame-pointer size TU.
// Identity (NF58, round 7): the map<EHeroTrainableStatType, long> tree; Ego_r.pdb members of this type:
//   CHeroAbilityStatPackage::StatThresholds. Landed callers of its insert_unique 0x00463E2E: 0x00463D18 GFSerialiseMapBinaryIn<long,
//   EHeroTrainableStatType, std::less<EHeroTr; 0x00851CC0 CRetailMap_00851cc0::Add.
//   Its callee _M_insert 0x00463EB4 is the same instance. STLport 4.6.2 source, descriptive-by-library. Was the stand-in
//   `CRetailTree_longkey_ins00463eb4::insert`; now an explicit instantiation over stlport/fable_map.h (FABLE_RETAIL_STLP_MAP body;
//   FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ: out-of-line _M_decrement), the landed 0x0042CEE3 pattern.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ
#include "stlport/fable_map.h"
#include "engine/EHeroTrainableStatType.h"
namespace std {
template pair<_Rb_tree<EHeroTrainableStatType, pair<EHeroTrainableStatType const, long>, _Select1st<pair<EHeroTrainableStatType const, long> >, less<EHeroTrainableStatType >, allocator<pair<EHeroTrainableStatType const, long> > >::iterator, bool> _Rb_tree<EHeroTrainableStatType, pair<EHeroTrainableStatType const, long>, _Select1st<pair<EHeroTrainableStatType const, long> >, less<EHeroTrainableStatType >, allocator<pair<EHeroTrainableStatType const, long> > >::insert_unique(const pair<EHeroTrainableStatType const, long>&);
}
