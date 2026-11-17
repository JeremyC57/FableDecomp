// std::_Rb_tree<unsigned long, pair<unsigned long const, unsigned long>, _Select1st<pair<unsigned long const, unsigned long> >, less<unsigned long >, allocator<pair<unsigned long const, unsigned long> > >::insert_unique(const value_type& v) -- STLport 4.6 _tree.c unique insert: descend on key < node key, step back with _Rb_global<bool>::_M_decrement (0x004294E2) unless at begin(), then _M_insert (0x004313E9) when the predecessor's key is less; pair<iterator, bool> through the hidden pointer
// Leaf: ?insert_unique@?$_Rb_tree@KU?$pair@$$CBKK@std@@U?$_Select1st@U?$pair@$$CBKK@std@@@2@U?$less@K@2@V?$allocator@U?$pair@$$CBKK@std@@@2@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@U?$pair@$$CBKK@std@@U?$_Nonconst_traits@U?$pair@$$CBKK@std@@@2@@std@@_N@2@ABU?$pair@$$CBKK@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00431363, 134 bytes, thiscall `ret 8`; frame-pointer size TU.
// Identity (NF58, round 7): the map<unsigned long, unsigned long> tree; Ego_r.pdb members of this type:
//   CEngineScenePrimitiveStatsEnumerator::Destination, CMorphingSprite::GraphicIndices, CUIDef::ActionMapAliases,
//   CUIMiscThingsDef::MapPaths (+1 more). Landed callers of its insert_unique 0x00431363: 0x004312B4 GFSerialiseMapBinaryIn<unsigned
//   long, unsigned long, std::less<unsigne; 0x005537F0 std::_Rb_tree<unsigned long, std::pair<unsigned long const, unsigned l;
//   0x009CDA70 CBankFile::InternalWriteEntryList. Landed callers of _M_insert besides 0x00431363: 0x005537F0 std::_Rb_tree<unsigned
//   long, std::pair<unsigned long const, unsigned l.
//   Its callee _M_insert 0x004313E9 is the same instance. STLport 4.6.2 source, descriptive-by-library. Was the stand-in
//   `CRetailTree_ulongkey_ins004313e9::insert`; now an explicit instantiation over stlport/fable_map.h (FABLE_RETAIL_STLP_MAP body;
//   FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ: out-of-line _M_decrement), the landed 0x0042CEE3 pattern.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ
#include "stlport/fable_map.h"

namespace std {
template pair<_Rb_tree<unsigned long, pair<unsigned long const, unsigned long>, _Select1st<pair<unsigned long const, unsigned long> >, less<unsigned long >, allocator<pair<unsigned long const, unsigned long> > >::iterator, bool> _Rb_tree<unsigned long, pair<unsigned long const, unsigned long>, _Select1st<pair<unsigned long const, unsigned long> >, less<unsigned long >, allocator<pair<unsigned long const, unsigned long> > >::insert_unique(const pair<unsigned long const, unsigned long>&);
}
