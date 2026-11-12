// std::_Rb_tree<CDisplayModeDesc, CDisplayModeDesc, std::_Identity<CDisplayModeDesc>, std::less<CDisplayModeDesc>, std::allocator<CDisplayModeDesc > >::_Rb_tree(const std::less<CDisplayModeDesc>&, const std::allocator<CDisplayModeDesc >&) -- STLport 4.6 tree constructor _Rb_tree(const _Compare&, const allocator_type&): allocate the header node, colour it red, root 0, leftmost/rightmost = header
// Leaf: ??0?$_Rb_tree@VCDisplayModeDesc@@V1@U?$_Identity@VCDisplayModeDesc@@@std@@U?$less@VCDisplayModeDesc@@@3@V?$allocator@VCDisplayModeDesc@@@3@@std@@QAE@ABU?$less@VCDisplayModeDesc@@@1@ABV?$allocator@VCDisplayModeDesc@@@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009C25F0, 56 bytes.
// Types (proven, agentFAM11, 2026-11-12): set<CDisplayModeDesc> (agentFAM07 proved this tree for 0x009C2B00 set<CDisplayModeDesc>::set, RENAMES 2026-11-11): CDisplayManager::EnumerateDisplayModes 0x009C02D0 builds a local set of display modes and calls this tree's _M_erase 0x009C1DC0, landed and bound as _Rb_tree<CDisplayModeDesc, CDisplayModeDesc, _Identity<..>, ...>; ??0 / ??1 of set<CDisplayModeDesc> are in the PDB name sets.
//   This _Rb_tree constructor 0x009C25F0 is the COMDAT right before the tree's ~_Rb_tree 0x009C2630, which calls that _M_erase 0x009C1DC0.
//   Was the placeholder std::_Rb_tree<CUnprovenKey_009c1dc0,CUnprovenValue_009c1dc0,CUnprovenKeyOfValue_009c1dc0,std::less<CUnprovenKey_009c1dc0>,std::allocator<CUnprovenValue_009c1dc0> >::_Rb_tree (leaf ??0?$_Rb_tree@UCUnprovenKey_009c1dc0@@UCUnprovenValue_009c1dc0@@UCUnprovenKeyOfValue_009c1dc0@@U?$less@UCUnprovenKey_009c1dc0@@@std@@V?$allocator@UCUnprovenValue_009c1dc0@@@5@@std@@QAE@ABU?$less@UCUnprovenKey_009c1dc0@@@1@ABV?$allocator@UCUnprovenValue_009c1dc0@@@1@@Z); name-only change: the same member, the per-instantiation placeholder types replaced by
//   the proven ones.
// Model: the shared stlport/fable_map.h (STLport 4.6 _Rb_tree / set; FABLE_RETAIL_STLP_MAP) and engine/CDisplayModeDesc.h; the explicit instantiation emits
//   the member as its own COMDAT, as retail kept every used inline member (no /OPT:REF).
#define FABLE_RETAIL_STLP_MAP
#include "stlport/fable_map.h"
#include "engine/CDisplayModeDesc.h"

template std::_Rb_tree<CDisplayModeDesc, CDisplayModeDesc, std::_Identity<CDisplayModeDesc>, std::less<CDisplayModeDesc>, std::allocator<CDisplayModeDesc > >::_Rb_tree(const std::less<CDisplayModeDesc>&, const std::allocator<CDisplayModeDesc >&);
