// std::_Rb_tree<unsigned __int64, std::pair<unsigned __int64 const, CThing*>, std::_Select1st<std::pair<unsigned __int64 const, CThing*> >, std::less<unsigned __int64>, std::allocator<std::pair<unsigned __int64 const, CThing*> > >::_Rb_tree(const std::less<unsigned __int64>&, const std::allocator<std::pair<unsigned __int64 const, CThing*> >&) -- STLport 4.6 tree constructor _Rb_tree(const _Compare&, const allocator_type&): allocate the header node, colour it red, root 0, leftmost/rightmost = header
// Leaf: ??0?$_Rb_tree@_KU?$pair@$$CB_KPAVCThing@@@std@@U?$_Select1st@U?$pair@$$CB_KPAVCThing@@@std@@@2@U?$less@_K@2@V?$allocator@U?$pair@$$CB_KPAVCThing@@@std@@@2@@std@@QAE@ABU?$less@_K@1@ABV?$allocator@U?$pair@$$CB_KPAVCThing@@@std@@@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00528CF0, 56 bytes.
// Types (proven, agentFAM11, 2026-11-12): map<unsigned __int64, CThing*> (agentFAM07 proved this tree for 0x00529DB0 map<unsigned __int64, CThing*>::~map, RENAMES 2026-11-11): CThingManager::UIDToThingMap (Ego_r.pdb member type map<unsigned __int64, CThing*>): the landed CThingManager::~CThingManager 0x00523440 calls this tree's _M_erase 0x005278A0 and binds it as _Rb_tree<unsigned __int64, pair<const unsigned __int64, CThing*>, ...>; node 0x20 = 0x10 + pair<const unsigned __int64, CThing*> (8-aligned, 0x10)..
//   This _Rb_tree constructor 0x00528CF0 is the COMDAT right before the tree's ~_Rb_tree 0x00528D30, which calls that _M_erase 0x005278A0.
//   Was the placeholder std::_Rb_tree<CUnprovenKey_005278a0,CUnprovenValue_005278a0,CUnprovenKeyOfValue_005278a0,std::less<CUnprovenKey_005278a0>,std::allocator<CUnprovenValue_005278a0> >::_Rb_tree (leaf ??0?$_Rb_tree@UCUnprovenKey_005278a0@@UCUnprovenValue_005278a0@@UCUnprovenKeyOfValue_005278a0@@U?$less@UCUnprovenKey_005278a0@@@std@@V?$allocator@UCUnprovenValue_005278a0@@@5@@std@@QAE@ABU?$less@UCUnprovenKey_005278a0@@@1@ABV?$allocator@UCUnprovenValue_005278a0@@@1@@Z); name-only change: the same member, the per-instantiation placeholder types replaced by
//   the proven ones.
// Model: the shared stlport/fable_map.h (STLport 4.6 _Rb_tree / map; FABLE_RETAIL_STLP_MAP); the explicit instantiation emits
//   the member as its own COMDAT, as retail kept every used inline member (no /OPT:REF).
#define FABLE_RETAIL_STLP_MAP
class CThing;
#include "stlport/fable_map.h"

template std::_Rb_tree<unsigned __int64, std::pair<unsigned __int64 const, CThing*>, std::_Select1st<std::pair<unsigned __int64 const, CThing*> >, std::less<unsigned __int64>, std::allocator<std::pair<unsigned __int64 const, CThing*> > >::_Rb_tree(const std::less<unsigned __int64>&, const std::allocator<std::pair<unsigned __int64 const, CThing*> >&);
