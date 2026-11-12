// std::__distance<std::_Rb_tree_iterator<NScreenEffect::CScreenEffectPriorityStack::CStackEntry*, ::std::_Nonconst_traits<NScreenEffect::CScreenEffectPriorityStack::CStackEntry*>>>(std::_Rb_tree_iterator<NScreenEffect::CScreenEffectPriorityStack::CStackEntry*, ::std::_Nonconst_traits<NScreenEffect::CScreenEffectPriorityStack::CStackEntry*>> const &, std::_Rb_tree_iterator<NScreenEffect::CScreenEffectPriorityStack::CStackEntry*, ::std::_Nonconst_traits<NScreenEffect::CScreenEffectPriorityStack::CStackEntry*>> const &, std::input_iterator_tag const &) -- STLport 4.6 __distance(first, last, input_iterator_tag): count increments from first to last (operator++ inlined)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00882EF0, 78 bytes.
// Types (proven, agentFAM11, 2026-11-12): set<NScreenEffect::CScreenEffectPriorityStack::CStackEntry*> (_Nonconst_traits<CStackEntry*>). Sources: (1) retail: the landed
//   real-typed erase(first,last) 0x00884710 of this set calls this __distance and its relocation binds exactly this spelling
//   (rebuild/oracles/reloc_bindings.tsv); the set's ctor 0x00883D20 is proven the same set; (2) Ego_r.pdb CScreenEffectPriorityStack::RadialBlurKeys /
//   DisplacementKeys / ScreenFilterKeys are set<CStackEntry*>, and FableWin.pdb has _Tset_traits<CStackEntry*>.
//   Was the placeholder std::__distance<std::_Rb_tree_iterator<CUnprovenValue_00882ef0,CUnprovenTraits_00882ef0<CUnprovenValue_00882ef0> > > (leaf ??$__distance@U?$_Rb_tree_iterator@UCUnprovenValue_00882ef0@@U?$CUnprovenTraits_00882ef0@UCUnprovenValue_00882ef0@@@@@std@@@std@@YAHABU?$_Rb_tree_iterator@UCUnprovenValue_00882ef0@@U?$CUnprovenTraits_00882ef0@UCUnprovenValue_00882ef0@@@@@0@0ABUinput_iterator_tag@0@@Z); name-only change: the
//   retail body, now on the shared tree model, which carries the proven types (the local model text cannot name the nested class).
// Iterator traits: _Nonconst_traits (the spelling the landed caller binds).
// Model: the shared stlport/fable_map.h tree iterator and the fable_containers.h STLport 4.6 __distance (FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE),
//   with the element class from the shared engine/NScreenEffect_CScreenEffectPriorityStack.h (FABLE_RETAIL_CSCREENEFFECTPRIORITYSTACK_FULL);
//   explicit instantiation emits the member as its own COMDAT, as retail kept every used inline member (no /OPT:REF). Replaces the invented stand-in (was _global::Distance_CRetailTreeIterator_00882ef0).
// Leaf: ??$__distance@U?$_Rb_tree_iterator@PAUCStackEntry@CScreenEffectPriorityStack@NScreenEffect@@U?$_Nonconst_traits@PAUCStackEntry@CScreenEffectPriorityStack@NScreenEffect@@@std@@@std@@@std@@YAHABU?$_Rb_tree_iterator@PAUCStackEntry@CScreenEffectPriorityStack@NScreenEffect@@U?$_Nonconst_traits@PAUCStackEntry@CScreenEffectPriorityStack@NScreenEffect@@@std@@@0@0ABUinput_iterator_tag@0@@Z
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE
#define FABLE_RETAIL_CSCREENEFFECTPRIORITYSTACK_FULL
#include "stlport/fable_map.h"
#include "engine/NScreenEffect_CScreenEffectPriorityStack.h"

typedef NScreenEffect::CScreenEffectPriorityStack::CStackEntry* CStackEntryKey;
typedef std::_Rb_tree_iterator<CStackEntryKey, std::_Nonconst_traits<CStackEntryKey> > CStackEntrySetIterator;
template ptrdiff_t std::__distance<CStackEntrySetIterator>(const CStackEntrySetIterator&, const CStackEntrySetIterator&, const std::input_iterator_tag&);
