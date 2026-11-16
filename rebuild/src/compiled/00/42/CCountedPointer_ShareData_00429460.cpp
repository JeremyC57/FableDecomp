#pragma optimize("s",on)
// CCountedPointer<NUISystem::CText>::ShareData(NUISystem::CText*, CCPPointerInfo*) -- share another pointer's block: when the block
// differs, release the current one (ReleaseData 0x0042925C), take the new data and block, and add a reference
// Leaf: ?ShareData@?$CCountedPointer@VCText@NUISystem@@@@QAEXPAVCText@NUISystem@@PAVCCPPointerInfo@@@Z
// NF39 (2026-11-16): moved off its CCPtr stand-in (CCPtr::ShareData calling a made-up ReleaseCur_0042925c) onto the shared
//   engine/CCountedPointer.h (FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA, the NUISystem TU's ReleaseData). FableWin.pdb
//   ?ShareData@?$CCountedPointer@VCText@NUISystem@@@@QAEXPAVCText@NUISystem@@PAVCCPPointerInfo@@@Z; the landed callers bind it so
//   (reloc_bindings: 0x005729D8). The catalog's "CCountedPointer<NUISystem::CAction>::ShareData" is a propagated label.
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include "engine/CCountedPointer.h"
namespace NUISystem { class CText; }
template void CCountedPointer<NUISystem::CText>::ShareData(NUISystem::CText*, CCPPointerInfo*);
