// std::_Rb_tree<EWorldMapNameGraphic, pair<EWorldMapNameGraphic const, unsigned long>, _Select1st<pair<EWorldMapNameGraphic const, unsigned long> >, less<EWorldMapNameGraphic >, allocator<pair<EWorldMapNameGraphic const, unsigned long> > >::insert_unique(const value_type& v) -- STLport 4.6 _tree.c unique insert: descend on key < node key, step back with _Rb_global<bool>::_M_decrement (0x004294E2) unless at begin(), then _M_insert (0x004647B9) when the predecessor's key is less; pair<iterator, bool> through the hidden pointer
// Leaf: ?insert_unique@?$_Rb_tree@W4EWorldMapNameGraphic@@U?$pair@$$CBW4EWorldMapNameGraphic@@K@std@@U?$_Select1st@U?$pair@$$CBW4EWorldMapNameGraphic@@K@std@@@3@U?$less@W4EWorldMapNameGraphic@@@3@V?$allocator@U?$pair@$$CBW4EWorldMapNameGraphic@@K@std@@@3@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@U?$pair@$$CBW4EWorldMapNameGraphic@@K@std@@U?$_Nonconst_traits@U?$pair@$$CBW4EWorldMapNameGraphic@@K@std@@@2@@std@@_N@2@ABU?$pair@$$CBW4EWorldMapNameGraphic@@K@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00464733, 134 bytes, thiscall `ret 8`; frame-pointer size TU.
// Identity (NF58, round 7): the map<EWorldMapNameGraphic, unsigned long> tree; Ego_r.pdb members of this type:
//   CUILocaleGraphicsDef::WorldMapRegionName. Landed callers of its insert_unique 0x00464733: 0x0046461D
//   GFSerialiseMapBinaryIn<unsigned long, EWorldMapNameGraphic, std::less<.
//   Its callee _M_insert 0x004647B9 is the same instance. STLport 4.6.2 source, descriptive-by-library. Was the stand-in
//   `CRetailTree_longkey_ins004647b9::insert`; now an explicit instantiation over stlport/fable_map.h (FABLE_RETAIL_STLP_MAP body;
//   FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ: out-of-line _M_decrement), the landed 0x0042CEE3 pattern.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ
#include "stlport/fable_map.h"
enum EWorldMapNameGraphic;
namespace std {
template pair<_Rb_tree<EWorldMapNameGraphic, pair<EWorldMapNameGraphic const, unsigned long>, _Select1st<pair<EWorldMapNameGraphic const, unsigned long> >, less<EWorldMapNameGraphic >, allocator<pair<EWorldMapNameGraphic const, unsigned long> > >::iterator, bool> _Rb_tree<EWorldMapNameGraphic, pair<EWorldMapNameGraphic const, unsigned long>, _Select1st<pair<EWorldMapNameGraphic const, unsigned long> >, less<EWorldMapNameGraphic >, allocator<pair<EWorldMapNameGraphic const, unsigned long> > >::insert_unique(const pair<EWorldMapNameGraphic const, unsigned long>&);
}
