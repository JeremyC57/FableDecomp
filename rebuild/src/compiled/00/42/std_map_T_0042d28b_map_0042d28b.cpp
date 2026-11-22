// std::map<CCharString, CCountedPointer<NUISystem::CComponent> >::map() -- STLport 4.6 map no-argument constructor of NUISystem::CManager's BastardChildren +0x88: the Compare() and allocator() temporaries (one reused [ebp-1] slot) passed to the tree constructor _Rb_tree(comp, alloc) (out of line)
// Leaf: ??0?$map@VCCharString@@V?$CCountedPointer@VCComponent@NUISystem@@@@U?$less@VCCharString@@@std@@V?$allocator@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCComponent@NUISystem@@@@@std@@@4@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042D28B, 25 bytes, frame-pointer size-optimised TU.
// Name (NF227, 2026-11-22, q12577): the STLport spelling the header produces (FABLE_RETAIL_STLP_MAP: `map() : _M_t(Compare(), A()) {}`;
//   STL instances have no PDB names: EXTRA_RULES 2026-11-20 "descriptive-by-library"). NF226's port of the CManager constructor
//   0x0041E3F6 calls it for BastardChildren +0x88 and its pick-any copy equals this address (relocations alike); 0x0041E3F6 binds this spelling. Was the
//   placeholder ?map@map_T_0042d28b@std@@QAEPAU12@XZ (catalog std::map<T_0042d28b>::map; first line was: std::map<T_0042d28b>::map() -- STLport map default constructor (Compare() and allocator() temporaries passed to _Rb_t...).
// Model: engine/NUISystem_CManager.h (the member's map type) and stlport/fable_map.h; the explicit instantiation emits the member as
//   its own COMDAT (the 0x008353B0 pattern). Defines and pragma as 0x0041E3F6 (the same retail TU).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_STLP_MAP
#pragma optimize("s", on)
#include "engine/CCountedPointer.h"
#include "engine/NUISystem_CManager.h"
template std::map<CCharString, CCountedPointer<NUISystem::CComponent> >::map();
