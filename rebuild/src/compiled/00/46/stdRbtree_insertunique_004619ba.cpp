// std::_Rb_tree<EDamageAttribute, pair<EDamageAttribute const, float>, _Select1st<pair<EDamageAttribute const, float> >, less<EDamageAttribute >, allocator<pair<EDamageAttribute const, float> > >::insert_unique(const value_type& v) -- STLport 4.6 _tree.c unique insert: descend on key < node key, step back with _Rb_global<bool>::_M_decrement (0x004294E2) unless at begin(), then _M_insert (0x00461A40) when the predecessor's key is less; pair<iterator, bool> through the hidden pointer
// Leaf: ?insert_unique@?$_Rb_tree@W4EDamageAttribute@@U?$pair@$$CBW4EDamageAttribute@@M@std@@U?$_Select1st@U?$pair@$$CBW4EDamageAttribute@@M@std@@@3@U?$less@W4EDamageAttribute@@@3@V?$allocator@U?$pair@$$CBW4EDamageAttribute@@M@std@@@3@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@U?$pair@$$CBW4EDamageAttribute@@M@std@@U?$_Nonconst_traits@U?$pair@$$CBW4EDamageAttribute@@M@std@@@2@@std@@_N@2@ABU?$pair@$$CBW4EDamageAttribute@@M@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004619BA, 134 bytes, thiscall `ret 8`; frame-pointer size TU.
// Identity (NF58, round 7): the map<EDamageAttribute, float> tree; Ego_r.pdb members of this type: CArmourDef::DamageTypeResponse,
//   CArmourDef::ArmourThresholds, CComboMultiplierData::ComboMultiplier, CHitParameters::DamageMap. Landed callers of its insert_unique
//   0x004619BA: 0x0046190A GFSerialiseMapBinaryIn<long,long,std::GFSerialiseMapBinaryIn<long,long; 0x006DBE00
//   CPersistTraits<CComboMultiplierData>::TransferBinaryIn; 0x00868CA0 CHitParameters::AddDamageWithAttribute.
//   Its callee _M_insert 0x00461A40 is the same instance. STLport 4.6.2 source, descriptive-by-library. Was the stand-in
//   `CRetailTree_longkey_ins00461a40::insert`; now an explicit instantiation over stlport/fable_map.h (FABLE_RETAIL_STLP_MAP body;
//   FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ: out-of-line _M_decrement), the landed 0x0042CEE3 pattern.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ
#include "stlport/fable_map.h"
#include "engine/EDamageAttribute.h"
namespace std {
template pair<_Rb_tree<EDamageAttribute, pair<EDamageAttribute const, float>, _Select1st<pair<EDamageAttribute const, float> >, less<EDamageAttribute >, allocator<pair<EDamageAttribute const, float> > >::iterator, bool> _Rb_tree<EDamageAttribute, pair<EDamageAttribute const, float>, _Select1st<pair<EDamageAttribute const, float> >, less<EDamageAttribute >, allocator<pair<EDamageAttribute const, float> > >::insert_unique(const pair<EDamageAttribute const, float>&);
}
