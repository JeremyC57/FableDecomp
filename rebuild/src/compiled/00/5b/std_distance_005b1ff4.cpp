// std::distance(const _Rb_tree_iterator<..>& first, const _Rb_tree_iterator<..>& last) -- STLport 4.6 _iterator_base.h: forward to
// __distance(first, last, input_iterator_tag()) over the CTCInventoryTrade map<NUISystem::CComponent*, pair<long, long> > iterator
// Leaf: ??$distance@U?$_Rb_tree_iterator@U?$pair@QAVCComponent@NUISystem@@U?$pair@JJ@std@@@std@@U?$_Nonconst_traits@U?$pair@QAVCComponent@NUISystem@@U?$pair@JJ@std@@@std@@@2@@std@@@std@@YAHABU?$_Rb_tree_iterator@U?$pair@QAVCComponent@NUISystem@@U?$pair@JJ@std@@@std@@U?$_Nonconst_traits@U?$pair@QAVCComponent@NUISystem@@U?$pair@JJ@std@@@std@@@2@@0@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005B1FF4, __cdecl `ret` (size TU, frame-pointer off: the tag temporary is a stack byte whose
// address is pushed); calls __distance 0x005B1AFD.
// Identity (NF42, round 5): STLport 4.6.2 `distance(const _InputIterator&, const _InputIterator&) { return __distance(__first, __last,
// _STLP_ITERATOR_CATEGORY(..)) }` with the tree iterator's category taken as input_iterator_tag as in fable_containers.h's model;
// descriptive-by-library. No retail E8 caller (an emitted COMDAT). Was the placeholder `CEngineSceneGrid::GetCellBoundingBox` calling
// `helper_5b1afd`; now an explicit instantiation over the shared STLport headers.
#pragma optimize("s", on)
// NF419 (2026-11-25): no `#pragma optimize("y", off)`: the leaf is byte- and relocation-identical without it (size mode keeps
//   its frame); it only framed end-of-TU template and compiler-generated copies (each copy checked in the T3 nf416 audit).
#define FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE
#include "stlport/fable_map.h"
namespace NUISystem { class CComponent; }
namespace std {
typedef _Rb_tree_iterator<pair<NUISystem::CComponent* const, pair<long, long> >, _Nonconst_traits<pair<NUISystem::CComponent* const, pair<long, long> > > > _TradeCountIterator;
template ptrdiff_t __cdecl distance<_TradeCountIterator>(const _TradeCountIterator&, const _TradeCountIterator&);
}
