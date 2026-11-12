// std::_Rb_tree<NUISystem::CComponent*, NUISystem::CComponent*, std::_Identity<NUISystem::CComponent*>, std::less<NUISystem::CComponent*>, std::allocator<NUISystem::CComponent* > >::_Rb_tree(const std::less<NUISystem::CComponent*>&, const std::allocator<NUISystem::CComponent* >&) -- STLport 4.6 tree constructor _Rb_tree(const _Compare&, const allocator_type&): allocate the header node, colour it red, root 0, leftmost/rightmost = header
// Leaf: ??0?$_Rb_tree@PAVCComponent@NUISystem@@PAV12@U?$_Identity@PAVCComponent@NUISystem@@@std@@U?$less@PAVCComponent@NUISystem@@@4@V?$allocator@PAVCComponent@NUISystem@@@4@@std@@QAE@ABU?$less@PAVCComponent@NUISystem@@@1@ABV?$allocator@PAVCComponent@NUISystem@@@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00535610, 56 bytes.
// Types (proven, agentFAM11, 2026-11-12): set<NUISystem::CComponent*> (agentFAM07 proved this tree for 0x00535750 set<NUISystem::CComponent*>::set, RENAMES 2026-11-11): NUISystem::CComponent::PositionChildren (Ego_r.pdb set<NUISystem::CComponent*>, the class's only set; its other tree, the map<unsigned long, CUIState> States, has a larger node): ~CComponent 0x00532D90 calls this tree's _M_erase 0x00534C60 (0x00532FC6).
//   This _Rb_tree constructor 0x00535610 is the COMDAT right before the tree's ~_Rb_tree 0x00535650, which calls that _M_erase 0x00534C60.
//   Was the placeholder std::_Rb_tree<CUnprovenKey_00534c60,CUnprovenValue_00534c60,CUnprovenKeyOfValue_00534c60,std::less<CUnprovenKey_00534c60>,std::allocator<CUnprovenValue_00534c60> >::_Rb_tree (leaf ??0?$_Rb_tree@UCUnprovenKey_00534c60@@UCUnprovenValue_00534c60@@UCUnprovenKeyOfValue_00534c60@@U?$less@UCUnprovenKey_00534c60@@@std@@V?$allocator@UCUnprovenValue_00534c60@@@5@@std@@QAE@ABU?$less@UCUnprovenKey_00534c60@@@1@ABV?$allocator@UCUnprovenValue_00534c60@@@1@@Z); name-only change: the same member, the per-instantiation placeholder types replaced by
//   the proven ones.
// Model: the shared stlport/fable_map.h (STLport 4.6 _Rb_tree / set; FABLE_RETAIL_STLP_MAP); the explicit instantiation emits
//   the member as its own COMDAT, as retail kept every used inline member (no /OPT:REF).
#define FABLE_RETAIL_STLP_MAP
namespace NUISystem { class CComponent; }
#include "stlport/fable_map.h"

template std::_Rb_tree<NUISystem::CComponent*, NUISystem::CComponent*, std::_Identity<NUISystem::CComponent*>, std::less<NUISystem::CComponent*>, std::allocator<NUISystem::CComponent* > >::_Rb_tree(const std::less<NUISystem::CComponent*>&, const std::allocator<NUISystem::CComponent* >&);
