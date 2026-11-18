#pragma optimize("s",on)
// CCountedPointer<NUISystem::CList>::ShareData(CList* data, CCPPointerInfo* info) -- unless already sharing that count block: release the
//   current reference (out-of-line ReleaseData 0x00429288), take the new data / block, count it if any
// Leaf: ?ShareData@?$CCountedPointer@VCList@NUISystem@@@@QAEXPAVCList@NUISystem@@PAVCCPPointerInfo@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00429486, 38 bytes, `ret 8`.
// NF75 (round 7, land1117fu q10944): the CList instance (catalog CCountedPointer<NUISystem::CList>::ShareData; called by
//   CCountedPointer<CList>::SetStaticCast<CComponent> 0x0042A2DB; landed 0x005729D8 binds this spelling; FableWin.pdb has the symbol). The
//   byte-identical 0x0059AA86 is the CScrollableList instance. Was a CCPtr stand-in. engine/CCountedPointer.h's ShareData body with
//   ReleaseData declared only (out of line, as retail calls it).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY
#include "engine/CCountedPointer.h"
namespace NUISystem { class CList; }
template void CCountedPointer<NUISystem::CList>::ShareData(NUISystem::CList*, CCPPointerInfo*);
