// std::_Rb_tree<NUISystem::EEvent, NUISystem::EEvent, std::_Identity<NUISystem::EEvent>, std::less<NUISystem::EEvent>, std::allocator<NUISystem::EEvent > >::_Rb_tree(const std::less<NUISystem::EEvent>&, const std::allocator<NUISystem::EEvent >&) -- STLport 4.6 tree constructor _Rb_tree(const _Compare&, const allocator_type&): allocate the header node, colour it red, root 0, leftmost/rightmost = header
// Leaf: ??0?$_Rb_tree@W4EEvent@NUISystem@@W412@U?$_Identity@W4EEvent@NUISystem@@@std@@U?$less@W4EEvent@NUISystem@@@4@V?$allocator@W4EEvent@NUISystem@@@4@@std@@QAE@ABU?$less@W4EEvent@NUISystem@@@1@ABV?$allocator@W4EEvent@NUISystem@@@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052E010, 56 bytes.
// Types (proven, agentFAM11, 2026-11-12): set<NUISystem::EEvent> (agentFAM07 proved this tree for 0x0052E1A0 set<NUISystem::EEvent>::set, RENAMES 2026-11-11): NUISystem::CObserver::EventsToObserve (Ego_r.pdb set<NUISystem::EEvent>, CObserver +4): the landed CObserver::IgnoreAllEvents 0x0052D9A0 calls this tree's _M_erase 0x0052DCA0 and binds it as _Rb_tree<NUISystem::EEvent, NUISystem::EEvent, _Identity<..>, ...>.
//   This _Rb_tree constructor 0x0052E010 is the COMDAT right before the tree's ~_Rb_tree 0x0052E050, which calls that _M_erase 0x0052DCA0.
//   Was the placeholder std::_Rb_tree<CUnprovenKey_0052dca0,CUnprovenValue_0052dca0,CUnprovenKeyOfValue_0052dca0,std::less<CUnprovenKey_0052dca0>,std::allocator<CUnprovenValue_0052dca0> >::_Rb_tree (leaf ??0?$_Rb_tree@UCUnprovenKey_0052dca0@@UCUnprovenValue_0052dca0@@UCUnprovenKeyOfValue_0052dca0@@U?$less@UCUnprovenKey_0052dca0@@@std@@V?$allocator@UCUnprovenValue_0052dca0@@@5@@std@@QAE@ABU?$less@UCUnprovenKey_0052dca0@@@1@ABV?$allocator@UCUnprovenValue_0052dca0@@@1@@Z); name-only change: the same member, the per-instantiation placeholder types replaced by
//   the proven ones.
// Model: the shared stlport/fable_map.h (STLport 4.6 _Rb_tree / set; FABLE_RETAIL_STLP_MAP) and engine/NUISystem_enums.h; the explicit instantiation emits
//   the member as its own COMDAT, as retail kept every used inline member (no /OPT:REF).
#define FABLE_RETAIL_STLP_MAP
#include "stlport/fable_map.h"
#include "engine/NUISystem_enums.h"

template std::_Rb_tree<NUISystem::EEvent, NUISystem::EEvent, std::_Identity<NUISystem::EEvent>, std::less<NUISystem::EEvent>, std::allocator<NUISystem::EEvent > >::_Rb_tree(const std::less<NUISystem::EEvent>&, const std::allocator<NUISystem::EEvent >&);
