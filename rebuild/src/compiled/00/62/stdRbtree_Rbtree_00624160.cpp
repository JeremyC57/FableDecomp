// std::_Rb_tree<CThing*, std::pair<CThing* const, NUISystem::CComponent*>, std::_Select1st<std::pair<CThing* const, NUISystem::CComponent*> >, std::less<CThing*>, std::allocator<std::pair<CThing* const, NUISystem::CComponent*> > >::_Rb_tree(const std::less<CThing*>&, const std::allocator<std::pair<CThing* const, NUISystem::CComponent*> >&) -- STLport 4.6 tree constructor _Rb_tree(const _Compare&, const allocator_type&): allocate the header node, colour it red, root 0, leftmost/rightmost = header
// Leaf: ??0?$_Rb_tree@PAVCThing@@U?$pair@QAVCThing@@PAVCComponent@NUISystem@@@std@@U?$_Select1st@U?$pair@QAVCThing@@PAVCComponent@NUISystem@@@std@@@3@U?$less@PAVCThing@@@3@V?$allocator@U?$pair@QAVCThing@@PAVCComponent@NUISystem@@@std@@@3@@std@@QAE@ABU?$less@PAVCThing@@@1@ABV?$allocator@U?$pair@QAVCThing@@PAVCComponent@NUISystem@@@std@@@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00624160, 56 bytes.
// Types (proven, agentFAM11, 2026-11-12): map<CThing*, NUISystem::CComponent*> (agentFAM07 proved this tree for 0x00624520 map<CThing*, NUISystem::CComponent*>::map, RENAMES 2026-11-11): CTCInventoryQuests::AvailableQuests (Ego_r.pdb map<CThing*, NUISystem::CComponent*>): the landed ~CTCInventoryQuests 0x0061A500 calls this tree's _M_erase 0x00623560 and binds it as _Rb_tree<CThing*, pair<CThing* const, NUISystem::CComponent*>, ...>.
//   This _Rb_tree constructor 0x00624160 is the COMDAT right before the tree's ~_Rb_tree 0x006241A0, which calls that _M_erase 0x00623560.
//   Was the placeholder std::_Rb_tree<CUnprovenKey_00623560,CUnprovenValue_00623560,CUnprovenKeyOfValue_00623560,std::less<CUnprovenKey_00623560>,std::allocator<CUnprovenValue_00623560> >::_Rb_tree (leaf ??0?$_Rb_tree@UCUnprovenKey_00623560@@UCUnprovenValue_00623560@@UCUnprovenKeyOfValue_00623560@@U?$less@UCUnprovenKey_00623560@@@std@@V?$allocator@UCUnprovenValue_00623560@@@5@@std@@QAE@ABU?$less@UCUnprovenKey_00623560@@@1@ABV?$allocator@UCUnprovenValue_00623560@@@1@@Z); name-only change: the same member, the per-instantiation placeholder types replaced by
//   the proven ones.
// Model: the shared stlport/fable_map.h (STLport 4.6 _Rb_tree / map; FABLE_RETAIL_STLP_MAP); the explicit instantiation emits
//   the member as its own COMDAT, as retail kept every used inline member (no /OPT:REF).
#define FABLE_RETAIL_STLP_MAP
class CThing;
namespace NUISystem { class CComponent; }
#include "stlport/fable_map.h"

template std::_Rb_tree<CThing*, std::pair<CThing* const, NUISystem::CComponent*>, std::_Select1st<std::pair<CThing* const, NUISystem::CComponent*> >, std::less<CThing*>, std::allocator<std::pair<CThing* const, NUISystem::CComponent*> > >::_Rb_tree(const std::less<CThing*>&, const std::allocator<std::pair<CThing* const, NUISystem::CComponent*> >&);
