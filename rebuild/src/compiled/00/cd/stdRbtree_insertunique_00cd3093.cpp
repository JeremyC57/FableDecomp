// std::_Rb_tree<int, pair<int const, CCharString>, _Select1st<pair<int const, CCharString> >, less<int >, allocator<pair<int const, CCharString> > >::insert_unique(const value_type& v) -- STLport 4.6 _tree.c unique insert: descend on key < node key, step back with _Rb_global<bool>::_M_decrement (0x004294E2) unless at begin(), then _M_insert (0x00CD300B) when the predecessor's key is less; pair<iterator, bool> through the hidden pointer
// Leaf: ?insert_unique@?$_Rb_tree@HU?$pair@$$CBHVCCharString@@@std@@U?$_Select1st@U?$pair@$$CBHVCCharString@@@std@@@2@U?$less@H@2@V?$allocator@U?$pair@$$CBHVCCharString@@@std@@@2@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@U?$pair@$$CBHVCCharString@@@std@@U?$_Nonconst_traits@U?$pair@$$CBHVCCharString@@@std@@@2@@std@@_N@2@ABU?$pair@$$CBHVCCharString@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CD3093, 134 bytes, thiscall `ret 8`; frame-pointer size TU.
// Identity (NF58, round 7): the map<int, CCharString> tree; no Ego_r.pdb member of this exact type; its two sources: both PDB name sets
//   carry the cutscene module's map<int, CCharString> (?PlayCutsceneSound@@YIXAAV?$map@HVCCharString@@...@Z, beside RunCutsceneMacro,
//   whose operator[] 0x00CD3A16 reaches this tree through the forwarder 0x00CD36C2 and the hinted insert_unique 0x00CD34C5), and
//   map<long, CCharString> is already the separate family 0x00472A05 / 0x0047297F (one instance, one address; REJECTED.tsv); node 0x18
//   copies the key and calls the CCharString copy 0x0099EC30 through the pair copy 0x00CD2395. Landed callers of its insert_unique
//   0x00CD3093: 0x00CD34C5 _global::RBTree_TransferAllocateInt. Landed callers of _M_insert besides 0x00CD3093: 0x00CD34C5
//   _global::RBTree_TransferAllocateInt.
//   Its callee _M_insert 0x00CD300B is the same instance. STLport 4.6.2 source, descriptive-by-library. Was the stand-in
//   `CRetailTree_longkey_ins00cd300b::insert`; now an explicit instantiation over stlport/fable_map.h (FABLE_RETAIL_STLP_MAP body;
//   FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ: out-of-line _M_decrement), the landed 0x0042CEE3 pattern.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ
#include "stlport/fable_map.h"
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
namespace std {
template pair<_Rb_tree<int, pair<int const, CCharString>, _Select1st<pair<int const, CCharString> >, less<int >, allocator<pair<int const, CCharString> > >::iterator, bool> _Rb_tree<int, pair<int const, CCharString>, _Select1st<pair<int const, CCharString> >, less<int >, allocator<pair<int const, CCharString> > >::insert_unique(const pair<int const, CCharString>&);
}
