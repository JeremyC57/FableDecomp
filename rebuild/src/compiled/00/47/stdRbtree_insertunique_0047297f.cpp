// std::_Rb_tree<long, pair<long const, CCharString>, _Select1st<pair<long const, CCharString> >, less<long >, allocator<pair<long const, CCharString> > >::insert_unique(const value_type& v) -- STLport 4.6 _tree.c unique insert: descend on key < node key, step back with _Rb_global<bool>::_M_decrement (0x004294E2) unless at begin(), then _M_insert (0x00472A05) when the predecessor's key is less; pair<iterator, bool> through the hidden pointer
// Leaf: ?insert_unique@?$_Rb_tree@JU?$pair@$$CBJVCCharString@@@std@@U?$_Select1st@U?$pair@$$CBJVCCharString@@@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJVCCharString@@@std@@@2@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@U?$pair@$$CBJVCCharString@@@std@@U?$_Nonconst_traits@U?$pair@$$CBJVCCharString@@@std@@@2@@std@@_N@2@ABU?$pair@$$CBJVCCharString@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0047297F, 134 bytes, thiscall `ret 8`; frame-pointer size TU.
// Identity (NF58, round 7): the map<long, CCharString> tree; Ego_r.pdb members of this type: CCreatureActionBase::CutSceneEvents,
//   CGlobalDef::ConversationCameras, CProfileManager::IDToProfileName. Landed callers of its insert_unique 0x0047297F: 0x00472896
//   _global::GFSerialiseMapBinaryIn.
//   Its callee _M_insert 0x00472A05 is the same instance. STLport 4.6.2 source, descriptive-by-library. Was the stand-in
//   `CRetailTree_longkey_ins00472a05::insert`; now an explicit instantiation over stlport/fable_map.h (FABLE_RETAIL_STLP_MAP body;
//   FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ: out-of-line _M_decrement), the landed 0x0042CEE3 pattern.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ
#include "stlport/fable_map.h"
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
namespace std {
template pair<_Rb_tree<long, pair<long const, CCharString>, _Select1st<pair<long const, CCharString> >, less<long >, allocator<pair<long const, CCharString> > >::iterator, bool> _Rb_tree<long, pair<long const, CCharString>, _Select1st<pair<long const, CCharString> >, less<long >, allocator<pair<long const, CCharString> > >::insert_unique(const pair<long const, CCharString>&);
}
