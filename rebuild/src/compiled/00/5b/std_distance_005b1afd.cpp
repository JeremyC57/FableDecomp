// std::__distance(const _Rb_tree_iterator<..>& first, const _Rb_tree_iterator<..>& last, const input_iterator_tag&) -- STLport 4.6
// _iterator_base.h input-iterator distance over the CTCInventoryTrade map<NUISystem::CComponent*, pair<long, long> > iterator: copy
// first, step it with _Rb_global<bool>::_M_increment (0x004292C0, out of line) until last, counting
// Leaf: ??$__distance@U?$_Rb_tree_iterator@U?$pair@QAVCComponent@NUISystem@@U?$pair@JJ@std@@@std@@U?$_Nonconst_traits@U?$pair@QAVCComponent@NUISystem@@U?$pair@JJ@std@@@std@@@2@@std@@@std@@YAHABU?$_Rb_tree_iterator@U?$pair@QAVCComponent@NUISystem@@U?$pair@JJ@std@@@std@@U?$_Nonconst_traits@U?$pair@QAVCComponent@NUISystem@@U?$pair@JJ@std@@@std@@@2@@0@0ABUinput_iterator_tag@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005B1AFD, 35 bytes, __cdecl `ret` (size TU: `pop ecx` cleanup).
// Identity (NF42, round 5): the landed _Rb_tree::erase(const key&) 0x005B2779 of that map (ItemsToSell, Ego_r.pdb member type)
// binds this spelling; STLport 4.6.2 source, descriptive-by-library (both PDBs are Dinkumware builds). Was the placeholder
// `Distance_005b1afd(const SIterator&, const SIterator&)`; now an explicit instantiation over stlport/fable_containers.h
// (FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE) and stlport/fable_map.h; the visible inline _M_increment stays a call in this size TU and
// lets VC7.1 keep last's node in esi across it (a declaration-only _M_increment re-reads it: 33 v 35 bytes).
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE
#include "stlport/fable_map.h"
namespace NUISystem { class CComponent; }
namespace std {
typedef _Rb_tree_iterator<pair<NUISystem::CComponent* const, pair<long, long> >, _Nonconst_traits<pair<NUISystem::CComponent* const, pair<long, long> > > > _TradeCountIterator;
template ptrdiff_t __cdecl __distance<_TradeCountIterator>(const _TradeCountIterator&, const _TradeCountIterator&, const input_iterator_tag&);
}
