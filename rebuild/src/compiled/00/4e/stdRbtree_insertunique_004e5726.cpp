// std::_Rb_tree<EClothingSuitPart, pair<EClothingSuitPart const, long>, _Select1st<pair<EClothingSuitPart const, long> >, less<EClothingSuitPart >, allocator<pair<EClothingSuitPart const, long> > >::insert_unique(const value_type& v) -- STLport 4.6 _tree.c unique insert: descend on key < node key, step back with _Rb_global<bool>::_M_decrement (0x004294E2) unless at begin(), then _M_insert (0x004E57AC) when the predecessor's key is less; pair<iterator, bool> through the hidden pointer
// Leaf: ?insert_unique@?$_Rb_tree@W4EClothingSuitPart@@U?$pair@$$CBW4EClothingSuitPart@@J@std@@U?$_Select1st@U?$pair@$$CBW4EClothingSuitPart@@J@std@@@3@U?$less@W4EClothingSuitPart@@@3@V?$allocator@U?$pair@$$CBW4EClothingSuitPart@@J@std@@@3@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@U?$pair@$$CBW4EClothingSuitPart@@J@std@@U?$_Nonconst_traits@U?$pair@$$CBW4EClothingSuitPart@@J@std@@@2@@std@@_N@2@ABU?$pair@$$CBW4EClothingSuitPart@@J@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E5726, 134 bytes, thiscall `ret 8`; frame-pointer size TU.
// Identity (NF58, round 7): the map<EClothingSuitPart, long> tree; Ego_r.pdb members of this type: CHeroSuitDef::SuitParts,
//   CTCInventoryClothing::CurrentClothingSelected. Landed callers of its insert_unique 0x004E5726: 0x004E5673
//   GFSerialiseMapBinaryIn<long, EClothingSuitPart, std::less<EClothingSui; 0x005BB5EF std::_Rb_tree<EClothingSuitPart,
//   std::pair<EClothingSuitPart const, lo. Landed callers of _M_insert besides 0x004E5726: 0x005BB5EF std::_Rb_tree<EClothingSuitPart,
//   std::pair<EClothingSuitPart const, lo.
//   Its callee _M_insert 0x004E57AC is the same instance. STLport 4.6.2 source, descriptive-by-library. Was the stand-in
//   `CRetailTree_longkey_ins004e57ac::insert`; now an explicit instantiation over stlport/fable_map.h (FABLE_RETAIL_STLP_MAP body;
//   FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ: out-of-line _M_decrement), the landed 0x0042CEE3 pattern.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ
#include "stlport/fable_map.h"
#include "engine/EClothingSuitPart.h"
namespace std {
template pair<_Rb_tree<EClothingSuitPart, pair<EClothingSuitPart const, long>, _Select1st<pair<EClothingSuitPart const, long> >, less<EClothingSuitPart >, allocator<pair<EClothingSuitPart const, long> > >::iterator, bool> _Rb_tree<EClothingSuitPart, pair<EClothingSuitPart const, long>, _Select1st<pair<EClothingSuitPart const, long> >, less<EClothingSuitPart >, allocator<pair<EClothingSuitPart const, long> > >::insert_unique(const pair<EClothingSuitPart const, long>&);
}
