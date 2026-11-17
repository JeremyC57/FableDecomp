// std::_Rb_tree<EOpinionReactionType, pair<EOpinionReactionType const, bool>, _Select1st<pair<EOpinionReactionType const, bool> >, less<EOpinionReactionType >, allocator<pair<EOpinionReactionType const, bool> > >::insert_unique(const value_type& v) -- STLport 4.6 _tree.c unique insert: descend on key < node key, step back with _Rb_global<bool>::_M_decrement (0x004294E2) unless at begin(), then _M_insert (0x0045E0FA) when the predecessor's key is less; pair<iterator, bool> through the hidden pointer
// Leaf: ?insert_unique@?$_Rb_tree@W4EOpinionReactionType@@U?$pair@$$CBW4EOpinionReactionType@@_N@std@@U?$_Select1st@U?$pair@$$CBW4EOpinionReactionType@@_N@std@@@3@U?$less@W4EOpinionReactionType@@@3@V?$allocator@U?$pair@$$CBW4EOpinionReactionType@@_N@std@@@3@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@U?$pair@$$CBW4EOpinionReactionType@@_N@std@@U?$_Nonconst_traits@U?$pair@$$CBW4EOpinionReactionType@@_N@std@@@2@@std@@_N@2@ABU?$pair@$$CBW4EOpinionReactionType@@_N@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045E074, 134 bytes, thiscall `ret 8`; frame-pointer size TU.
// Identity (NF58, round 7): the map<EOpinionReactionType, bool> tree; Ego_r.pdb members of this type:
//   COpinionReactionManagerDef::BlockFurtherReactions, COpinionReactionManagerDef::AllowSpeechOnNonPureAISpeaker,
//   COpinionReactionManagerDef::AllowWhileCarrying, COpinionReactionManagerDef::AllowWhileFollowingPlayer (+2 more). Landed callers of
//   its insert_unique 0x0045E074: 0x0045DF5E _global::GFSerialiseMapBinaryIn.
//   Its callee _M_insert 0x0045E0FA is the same instance. STLport 4.6.2 source, descriptive-by-library. Was the stand-in
//   `CRetailTree_longkey_ins0045e0fa::insert`; now an explicit instantiation over stlport/fable_map.h (FABLE_RETAIL_STLP_MAP body;
//   FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ: out-of-line _M_decrement), the landed 0x0042CEE3 pattern.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ
#include "stlport/fable_map.h"
enum EOpinionReactionType;
namespace std {
template pair<_Rb_tree<EOpinionReactionType, pair<EOpinionReactionType const, bool>, _Select1st<pair<EOpinionReactionType const, bool> >, less<EOpinionReactionType >, allocator<pair<EOpinionReactionType const, bool> > >::iterator, bool> _Rb_tree<EOpinionReactionType, pair<EOpinionReactionType const, bool>, _Select1st<pair<EOpinionReactionType const, bool> >, less<EOpinionReactionType >, allocator<pair<EOpinionReactionType const, bool> > >::insert_unique(const pair<EOpinionReactionType const, bool>&);
}
