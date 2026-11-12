// std::_Rb_tree<void*, std::pair<void* const, CRGBColour>, std::_Select1st<std::pair<void* const, CRGBColour> >, std::less<void*>, std::allocator<std::pair<void* const, CRGBColour> > >::_Rb_tree(const std::less<void*>&, const std::allocator<std::pair<void* const, CRGBColour> >&) -- STLport 4.6 tree constructor _Rb_tree(const _Compare&, const allocator_type&): allocate the header node, colour it red, root 0, leftmost/rightmost = header
// Leaf: ??0?$_Rb_tree@PAXU?$pair@QAXVCRGBColour@@@std@@U?$_Select1st@U?$pair@QAXVCRGBColour@@@std@@@2@U?$less@PAX@2@V?$allocator@U?$pair@QAXVCRGBColour@@@std@@@2@@std@@QAE@ABU?$less@PAX@1@ABV?$allocator@U?$pair@QAXVCRGBColour@@@std@@@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C3ED0, 56 bytes.
// Types (proven, agentFAM11, 2026-11-12): map<void*, CRGBColour> (agentFAM07 proved this tree for 0x004C42D0 map<void*, CRGBColour>::map, RENAMES 2026-11-11): NGraphicAppearance::CTCGraphicAppearance::TintColours (Ego_r.pdb map<void*, CRGBColour>, +28; the class's other map, map<void*, CMeshEffectPriorityEntry>, has the 0x20 node and _M_erase 0x004C4490): ~CTCGraphicAppearance 0x004C5DA0 calls this tree's _M_erase 0x004C2950 (0x004C5DDA), the only 0x18-node tree of the class.
//   This _Rb_tree constructor 0x004C3ED0 is the COMDAT right before the tree's ~_Rb_tree 0x004C3F10, which calls that _M_erase 0x004C2950.
//   Was the placeholder std::_Rb_tree<CUnprovenKey_004c2950,CUnprovenValue_004c2950,CUnprovenKeyOfValue_004c2950,std::less<CUnprovenKey_004c2950>,std::allocator<CUnprovenValue_004c2950> >::_Rb_tree (leaf ??0?$_Rb_tree@UCUnprovenKey_004c2950@@UCUnprovenValue_004c2950@@UCUnprovenKeyOfValue_004c2950@@U?$less@UCUnprovenKey_004c2950@@@std@@V?$allocator@UCUnprovenValue_004c2950@@@5@@std@@QAE@ABU?$less@UCUnprovenKey_004c2950@@@1@ABV?$allocator@UCUnprovenValue_004c2950@@@1@@Z); name-only change: the same member, the per-instantiation placeholder types replaced by
//   the proven ones.
// Model: the shared stlport/fable_map.h (STLport 4.6 _Rb_tree / map; FABLE_RETAIL_STLP_MAP) and engine/CRGBColour.h; the explicit instantiation emits
//   the member as its own COMDAT, as retail kept every used inline member (no /OPT:REF).
#define FABLE_RETAIL_STLP_MAP
#include "stlport/fable_map.h"
#include "engine/CRGBColour.h"

template std::_Rb_tree<void*, std::pair<void* const, CRGBColour>, std::_Select1st<std::pair<void* const, CRGBColour> >, std::less<void*>, std::allocator<std::pair<void* const, CRGBColour> > >::_Rb_tree(const std::less<void*>&, const std::allocator<std::pair<void* const, CRGBColour> >&);
