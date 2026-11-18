#pragma optimize("s",on)
// CCountedPointer<NUISystem::CScrollableList>::ShareData(CScrollableList* data, CCPPointerInfo* info) -- unless already sharing that
//   count block: release the current reference (out-of-line ReleaseData 0x0059A994), take the new data / block, count it if any
// Leaf: ?ShareData@?$CCountedPointer@VCScrollableList@NUISystem@@@@QAEXPAVCScrollableList@NUISystem@@PAVCCPPointerInfo@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0059AA86, 38 bytes, `ret 8`.
// NF75 (round 7, land1117fu q10944): the CScrollableList instance (Ego_d.pdb ui_frontend_manager.obj contributes it, FableWin.pdb
//   ?ShareData@?$CCountedPointer@VCScrollableList@NUISystem@@@@QAEXPAVCScrollableList@NUISystem@@PAVCCPPointerInfo@@@Z 0x02026782; called by
//   CFrontEndManager 0x00596CCE / 0x00597006, calls 0x0059A994). The byte-identical CList instance is 0x00429486. Was a CCPtr stand-in.
//   engine/CCountedPointer.h's ShareData body with ReleaseData declared only (out of line, as retail calls it).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY
#include "engine/CCountedPointer.h"
namespace NUISystem { class CScrollableList; }
template void CCountedPointer<NUISystem::CScrollableList>::ShareData(NUISystem::CScrollableList*, CCPPointerInfo*);
