// std::_Rb_tree<void*, pair<void* const, float>, _Select1st<pair<void* const, float> >, less<void* >, allocator<pair<void* const, float> > >::insert_unique(const value_type& v) -- STLport 4.6 _tree.c unique insert: descend on key < node key, step back with _Rb_global<bool>::_M_decrement (0x004294E2) unless at begin(), then _M_insert (0x005808A0) when the predecessor's key is less; pair<iterator, bool> through the hidden pointer
// Leaf: ?insert_unique@?$_Rb_tree@PAXU?$pair@QAXM@std@@U?$_Select1st@U?$pair@QAXM@std@@@2@U?$less@PAX@2@V?$allocator@U?$pair@QAXM@std@@@2@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@U?$pair@QAXM@std@@U?$_Nonconst_traits@U?$pair@QAXM@std@@@2@@std@@_N@2@ABU?$pair@QAXM@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00580D21, 134 bytes, thiscall `ret 8`; frame-pointer size TU.
// Identity (NF58, round 7): the map<void*, float> tree; Ego_r.pdb members of this type: CTCHaste::Multipliers. Landed callers of its
//   insert_unique 0x00580D21: 0x005812AE CBlockTable_005812ae::GetBlockData; 0x00694020 CCreatureActionBase::FrameUpdate; 0x006A02B0
//   CCreatureAction_UnsheatheItemFromInventoryWithHasteAction::FinishActio; 0x0078B410
//   CCreatureAction_HeroLoadRangedWeapon::Initialise; 0x008C7280 CCreatureAction_TrollGroundPound::Initialise; 0x008CDD40
//   CCreatureAction_ReleaseTimeSpell::FinishAction; 0x008E6780 CCreatureAction_CastStrengthMultiStrike::Initialise.
//   Its callee _M_insert 0x005808A0 is the same instance. STLport 4.6.2 source, descriptive-by-library. Was the stand-in
//   `CTCHaste::PausedSetInsert`; now an explicit instantiation over stlport/fable_map.h (FABLE_RETAIL_STLP_MAP body;
//   FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ: out-of-line _M_decrement), the landed 0x0042CEE3 pattern.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ
#include "stlport/fable_map.h"

namespace std {
template pair<_Rb_tree<void*, pair<void* const, float>, _Select1st<pair<void* const, float> >, less<void* >, allocator<pair<void* const, float> > >::iterator, bool> _Rb_tree<void*, pair<void* const, float>, _Select1st<pair<void* const, float> >, less<void* >, allocator<pair<void* const, float> > >::insert_unique(const pair<void* const, float>&);
}
