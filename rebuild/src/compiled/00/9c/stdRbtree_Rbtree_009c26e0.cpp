// std::_Rb_tree<ESurfaceMultisampleType, ESurfaceMultisampleType, std::_Identity<ESurfaceMultisampleType>, std::less<ESurfaceMultisampleType>, std::allocator<ESurfaceMultisampleType > >::_Rb_tree(const std::less<ESurfaceMultisampleType>&, const std::allocator<ESurfaceMultisampleType >&) -- STLport 4.6 tree constructor _Rb_tree(const _Compare&, const allocator_type&): allocate the header node, colour it red, root 0, leftmost/rightmost = header
// Leaf: ??0?$_Rb_tree@W4ESurfaceMultisampleType@@W41@U?$_Identity@W4ESurfaceMultisampleType@@@std@@U?$less@W4ESurfaceMultisampleType@@@3@V?$allocator@W4ESurfaceMultisampleType@@@3@@std@@QAE@ABU?$less@W4ESurfaceMultisampleType@@@1@ABV?$allocator@W4ESurfaceMultisampleType@@@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009C26E0, 56 bytes.
// Types (proven, agentFAM11, 2026-11-12): set<ESurfaceMultisampleType> (agentFAM07 proved this tree for 0x009C2C10 set<ESurfaceMultisampleType>::~set, RENAMES 2026-11-11): CDisplayManager::EnumerateAntiAliasingModes 0x009C05C0 builds a local set of the device's multisample types and calls this tree's _M_erase 0x009C1E00, landed and bound as _Rb_tree<ESurfaceMultisampleType, ESurfaceMultisampleType, _Identity<..>, ...>; ??0 / ??1 of set<ESurfaceMultisampleType> are in both PDB name sets; node 0x14..
//   This _Rb_tree constructor 0x009C26E0 is the COMDAT right before the tree's ~_Rb_tree 0x009C2720, which calls that _M_erase 0x009C1E00.
//   Was the placeholder std::_Rb_tree<CUnprovenKey_009c1e00,CUnprovenValue_009c1e00,CUnprovenKeyOfValue_009c1e00,std::less<CUnprovenKey_009c1e00>,std::allocator<CUnprovenValue_009c1e00> >::_Rb_tree (leaf ??0?$_Rb_tree@UCUnprovenKey_009c1e00@@UCUnprovenValue_009c1e00@@UCUnprovenKeyOfValue_009c1e00@@U?$less@UCUnprovenKey_009c1e00@@@std@@V?$allocator@UCUnprovenValue_009c1e00@@@5@@std@@QAE@ABU?$less@UCUnprovenKey_009c1e00@@@1@ABV?$allocator@UCUnprovenValue_009c1e00@@@1@@Z); name-only change: the same member, the per-instantiation placeholder types replaced by
//   the proven ones.
// Model: the shared stlport/fable_map.h (STLport 4.6 _Rb_tree / set; FABLE_RETAIL_STLP_MAP) and engine/ESurfaceMultisampleType.h; the explicit instantiation emits
//   the member as its own COMDAT, as retail kept every used inline member (no /OPT:REF).
#define FABLE_RETAIL_STLP_MAP
#include "stlport/fable_map.h"
#include "engine/ESurfaceMultisampleType.h"

template std::_Rb_tree<ESurfaceMultisampleType, ESurfaceMultisampleType, std::_Identity<ESurfaceMultisampleType>, std::less<ESurfaceMultisampleType>, std::allocator<ESurfaceMultisampleType > >::_Rb_tree(const std::less<ESurfaceMultisampleType>&, const std::allocator<ESurfaceMultisampleType >&);
