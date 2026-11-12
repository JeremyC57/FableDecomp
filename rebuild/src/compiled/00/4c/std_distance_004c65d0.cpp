// std::__distance<std::_Rb_tree_iterator< ::std::pair<void* const, CBulletTimeManager::CBulletTimeInfo>, ::std::_Nonconst_traits< ::std::pair<void* const, CBulletTimeManager::CBulletTimeInfo>>>>(std::_Rb_tree_iterator< ::std::pair<void* const, CBulletTimeManager::CBulletTimeInfo>, ::std::_Nonconst_traits< ::std::pair<void* const, CBulletTimeManager::CBulletTimeInfo>>> const &, std::_Rb_tree_iterator< ::std::pair<void* const, CBulletTimeManager::CBulletTimeInfo>, ::std::_Nonconst_traits< ::std::pair<void* const, CBulletTimeManager::CBulletTimeInfo>>> const &, std::input_iterator_tag const &) -- STLport 4.6 __distance(first, last, input_iterator_tag): count increments from first to last (operator++ inlined)
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C65D0, 78 bytes.
// Types (proven, agentFAM11, 2026-11-12): map<void*, CBulletTimeManager::CBulletTimeInfo> (_Nonconst_traits<pair<void* const, CBulletTimeInfo> >). Sources: (1) retail: the
//   landed real-typed erase(first,last) 0x004C6B60 of this tree calls this __distance and its relocation binds exactly this spelling
//   (rebuild/oracles/reloc_bindings.tsv); (2) Ego_r.pdb CBulletTimeManager::BulletTimeInfoMap is map<void*, CBulletTimeManager::CBulletTimeInfo>.
//   Was the placeholder std::__distance<std::_Rb_tree_iterator<CUnprovenValue_004c65d0,CUnprovenTraits_004c65d0<CUnprovenValue_004c65d0> > > (leaf ??$__distance@U?$_Rb_tree_iterator@UCUnprovenValue_004c65d0@@U?$CUnprovenTraits_004c65d0@UCUnprovenValue_004c65d0@@@@@std@@@std@@YAHABU?$_Rb_tree_iterator@UCUnprovenValue_004c65d0@@U?$CUnprovenTraits_004c65d0@UCUnprovenValue_004c65d0@@@@@0@0ABUinput_iterator_tag@0@@Z); name-only change: the
//   retail body, now on the shared tree model, which carries the proven types (the local model text cannot name the nested class).
// Iterator traits: _Nonconst_traits (the spelling the landed caller binds).
// Model: the shared stlport/fable_map.h tree iterator and the fable_containers.h STLport 4.6 __distance (FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE),
//   with the value class from the shared engine/CBulletTimeManager.h (FABLE_RETAIL_CBULLETTIMEMANAGER + _INTERPOLATE, which declares
//   CBulletTimeInfo); explicit instantiation emits the member as its own COMDAT, as retail kept every used inline member (no /OPT:REF). Replaces the invented stand-in (was _global::Distance_CRetailTreeIterator_004c65d0).
// Leaf: ??$__distance@U?$_Rb_tree_iterator@U?$pair@QAXVCBulletTimeInfo@CBulletTimeManager@@@std@@U?$_Nonconst_traits@U?$pair@QAXVCBulletTimeInfo@CBulletTimeManager@@@std@@@2@@std@@@std@@YAHABU?$_Rb_tree_iterator@U?$pair@QAXVCBulletTimeInfo@CBulletTimeManager@@@std@@U?$_Nonconst_traits@U?$pair@QAXVCBulletTimeInfo@CBulletTimeManager@@@std@@@2@@0@0ABUinput_iterator_tag@0@@Z
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_MAP_EMPTY
#define FABLE_RETAIL_STLP_SET_EMPTY
#define FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE
#define FABLE_RETAIL_CBULLETTIMEMANAGER
#define FABLE_RETAIL_CBULLETTIMEMANAGER_INTERPOLATE
#include "engine/CBulletTimeManager.h"

typedef std::pair<void* const, CBulletTimeManager::CBulletTimeInfo> CBulletTimeInfoMapValue;
typedef std::_Rb_tree_iterator<CBulletTimeInfoMapValue, std::_Nonconst_traits<CBulletTimeInfoMapValue> > CBulletTimeInfoMapIterator;
template ptrdiff_t std::__distance<CBulletTimeInfoMapIterator>(const CBulletTimeInfoMapIterator&, const CBulletTimeInfoMapIterator&, const std::input_iterator_tag&);
