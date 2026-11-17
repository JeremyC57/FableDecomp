// std::_Rb_tree<NUISystem::CComponent*, pair<NUISystem::CComponent* const, long>, ...>::insert_unique(const value_type& v) -- STLport 4.6
// _tree.c unique insert: descend on key < node key, step back with _Rb_global<bool>::_M_decrement (0x004294E2) unless at begin(), then
// _M_insert (0x0042B98B) when the predecessor's key is less; pair<iterator, bool> through the hidden pointer
// Leaf: ?insert_unique@?$_Rb_tree@PAVCComponent@NUISystem@@U?$pair@QAVCComponent@NUISystem@@J@std@@U?$_Select1st@U?$pair@QAVCComponent@NUISystem@@J@std@@@4@U?$less@PAVCComponent@NUISystem@@@4@V?$allocator@U?$pair@QAVCComponent@NUISystem@@J@std@@@4@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@U?$pair@QAVCComponent@NUISystem@@J@std@@U?$_Nonconst_traits@U?$pair@QAVCComponent@NUISystem@@J@std@@@2@@std@@_N@2@ABU?$pair@QAVCComponent@NUISystem@@J@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0042BA13, 134 bytes, thiscall `ret 8`; frame-pointer size TU.
// Identity (NF58, round 7): the map<NUISystem::CComponent*, long> tree (Ego_r.pdb CTCInventoryClothing::ClothingItemMap /
//   CTCInventoryWeapons::WeaponMenuItemMap); the landed hinted insert_unique 0x0042C082 of the same instance calls this address with
//   this spelling (+0x12D); its callee _M_insert 0x0042B98B is the same tree. STLport 4.6.2 source, descriptive-by-library. Was the
//   stand-in `CRetailTree_ulongkey_ins0042b98b::insert`; now an explicit instantiation over stlport/fable_map.h (FABLE_RETAIL_STLP_MAP
//   body; FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ: out-of-line _M_decrement), the landed 0x0042CEE3 pattern.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ
#include "stlport/fable_map.h"
namespace NUISystem { class CComponent; }
namespace std {
template pair<_Rb_tree<NUISystem::CComponent*, pair<NUISystem::CComponent* const, long>, _Select1st<pair<NUISystem::CComponent* const, long> >, less<NUISystem::CComponent*>, allocator<pair<NUISystem::CComponent* const, long> > >::iterator, bool> _Rb_tree<NUISystem::CComponent*, pair<NUISystem::CComponent* const, long>, _Select1st<pair<NUISystem::CComponent* const, long> >, less<NUISystem::CComponent*>, allocator<pair<NUISystem::CComponent* const, long> > >::insert_unique(const pair<NUISystem::CComponent* const, long>&);
}
