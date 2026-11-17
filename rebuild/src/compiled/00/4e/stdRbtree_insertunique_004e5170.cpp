// std::_Rb_tree<long, pair<long const, float>, _Select1st<pair<long const, float> >, less<long >, allocator<pair<long const, float> > >::insert_unique(const value_type& v) -- STLport 4.6 _tree.c unique insert: descend on key < node key, step back with _Rb_global<bool>::_M_decrement (0x004294E2) unless at begin(), then _M_insert (0x004E51F6) when the predecessor's key is less; pair<iterator, bool> through the hidden pointer
// Leaf: ?insert_unique@?$_Rb_tree@JU?$pair@$$CBJM@std@@U?$_Select1st@U?$pair@$$CBJM@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJM@std@@@2@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@U?$pair@$$CBJM@std@@U?$_Nonconst_traits@U?$pair@$$CBJM@std@@@2@@std@@_N@2@ABU?$pair@$$CBJM@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E5170, 134 bytes, thiscall `ret 8`; frame-pointer size TU.
// Identity (NF58, round 7): the map<long, float> tree; Ego_r.pdb members of this type:
//   CExperienceDef::ParticleScalesPerExperiencePoints, CExperienceDef::LifeTimeForDistanceThresholds. Landed callers of its
//   insert_unique 0x004E5170: 0x004E50C0 GFSerialiseMapBinaryIn<long,long,std::GFSerialiseMapBinaryIn<long,long.
//   Its callee _M_insert 0x004E51F6 is the same instance. STLport 4.6.2 source, descriptive-by-library. Was the stand-in
//   `CRetailTree_longkey_ins004e51f6::insert`; now an explicit instantiation over stlport/fable_map.h (FABLE_RETAIL_STLP_MAP body;
//   FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ: out-of-line _M_decrement), the landed 0x0042CEE3 pattern.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ
#include "stlport/fable_map.h"

namespace std {
template pair<_Rb_tree<long, pair<long const, float>, _Select1st<pair<long const, float> >, less<long >, allocator<pair<long const, float> > >::iterator, bool> _Rb_tree<long, pair<long const, float>, _Select1st<pair<long const, float> >, less<long >, allocator<pair<long const, float> > >::insert_unique(const pair<long const, float>&);
}
