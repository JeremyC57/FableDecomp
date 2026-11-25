// std::_Rb_tree<NUISystem::CComponent*, pair<NUISystem::CComponent* const, std::pair<CWideString, long> >, _Select1st<pair<NUISystem::CComponent* const, std::pair<CWideString, long> > >, less<NUISystem::CComponent* >, allocator<pair<NUISystem::CComponent* const, std::pair<CWideString, long> > > >::insert_unique(const value_type& v) -- STLport 4.6 _tree.c unique insert: descend on key < node key, step back with _Rb_global<bool>::_M_decrement (0x004294E2) unless at begin(), then _M_insert (0x005B24CD) when the predecessor's key is less; pair<iterator, bool> through the hidden pointer
// Leaf: ?insert_unique@?$_Rb_tree@PAVCComponent@NUISystem@@U?$pair@QAVCComponent@NUISystem@@U?$pair@VCWideString@@J@std@@@std@@U?$_Select1st@U?$pair@QAVCComponent@NUISystem@@U?$pair@VCWideString@@J@std@@@std@@@4@U?$less@PAVCComponent@NUISystem@@@4@V?$allocator@U?$pair@QAVCComponent@NUISystem@@U?$pair@VCWideString@@J@std@@@std@@@4@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@U?$pair@QAVCComponent@NUISystem@@U?$pair@VCWideString@@J@std@@@std@@U?$_Nonconst_traits@U?$pair@QAVCComponent@NUISystem@@U?$pair@VCWideString@@J@std@@@std@@@2@@std@@_N@2@ABU?$pair@QAVCComponent@NUISystem@@U?$pair@VCWideString@@J@std@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005B2555, 134 bytes, thiscall `ret 8`; frame-pointer size TU.
// Identity (NF58, round 7): the map<NUISystem::CComponent*, std::pair<CWideString, long>> tree; Ego_r.pdb members of this type:
//   CTCInventoryTrade::NumBuyableThings, CTCInventoryTrade::NumSellableThings, CTCInventoryTrade::NumWantedThings. Landed callers of
//   its insert_unique 0x005B2555: 0x005B2B76 std::_Rb_tree<NUISystem::CComponent *, std::pair<NUISystem::CComponent. Landed callers of
//   _M_insert besides 0x005B2555: 0x005B2B76 std::_Rb_tree<NUISystem::CComponent *, std::pair<NUISystem::CComponent.
//   Its callee _M_insert 0x005B24CD is the same instance. STLport 4.6.2 source, descriptive-by-library. Was the stand-in
//   `CRetailTree_ulongkey_ins005b24cd::insert`; now an explicit instantiation over stlport/fable_map.h (FABLE_RETAIL_STLP_MAP body;
//   FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ: out-of-line _M_decrement), the landed 0x0042CEE3 pattern.
// NF419 (2026-11-25): no `#pragma optimize("y", off)`: the leaf is byte- and relocation-identical without it (size mode keeps
//   its frame); it only framed end-of-TU template and compiler-generated copies (each copy checked in the T3 nf416 audit).
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ
#include "stlport/fable_map.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#include "fable_string.h"
namespace NUISystem { class CComponent; }
namespace std {
template pair<_Rb_tree<NUISystem::CComponent*, pair<NUISystem::CComponent* const, std::pair<CWideString, long> >, _Select1st<pair<NUISystem::CComponent* const, std::pair<CWideString, long> > >, less<NUISystem::CComponent* >, allocator<pair<NUISystem::CComponent* const, std::pair<CWideString, long> > > >::iterator, bool> _Rb_tree<NUISystem::CComponent*, pair<NUISystem::CComponent* const, std::pair<CWideString, long> >, _Select1st<pair<NUISystem::CComponent* const, std::pair<CWideString, long> > >, less<NUISystem::CComponent* >, allocator<pair<NUISystem::CComponent* const, std::pair<CWideString, long> > > >::insert_unique(const pair<NUISystem::CComponent* const, std::pair<CWideString, long> >&);
}
