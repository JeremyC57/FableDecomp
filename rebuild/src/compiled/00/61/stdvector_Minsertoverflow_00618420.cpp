// std::vector<CTCInventoryMap::CUIRegionInfo>::_M_insert_overflow(CUIRegionInfo*, const CUIRegionInfo&, const __false_type&, unsigned int, bool) -- STLport 4.6 vector growth path: allocate max(2*size, size+n), copy the prefix, construct n copies of the value, copy the suffix unless appending, free the old block
// Flags: /O2 /Oy /W3
// Leaf: ?_M_insert_overflow@?$vector@VCUIRegionInfo@CTCInventoryMap@@V?$allocator@VCUIRegionInfo@CTCInventoryMap@@@std@@@std@@IAEXPAVCUIRegionInfo@CTCInventoryMap@@ABV34@ABU__false_type@2@I_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00618420. Called from 0x00605721 (push_back on CTCInventoryMap::UIRegionsInfo, +0x198) with
//   (finish, &CUIRegionInfo(), __false_type, 1, true); the element is the 40-byte Ego_r.pdb CTCInventoryMap::CUIRegionInfo.
// Name: the compiler's spelling of the STLport 4.6 member for vector<CTCInventoryMap::CUIRegionInfo> (descriptive-by-library: both PDBs
//   are Dinkumware builds; the element type is the Ego_r.pdb member type of UIRegionsInfo, the template arguments the callers prove).
// NF41 (2026-11-16): was a local `N00618420::vector<Element>` model; now one explicit instantiation on the shared
//   stlport/fable_containers.h and engine/CTCInventoryMap.h (the CUIRegionInfo element), as the landed 0x00CBC130 does.
#define FABLE_RETAIL_STLP_UNINIT_COPY_FORCEINLINE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CTCInventoryMap.h"
template void std::vector<CTCInventoryMap::CUIRegionInfo>::_M_insert_overflow(CTCInventoryMap::CUIRegionInfo*, const CTCInventoryMap::CUIRegionInfo&, const std::__false_type&, unsigned int, bool);
