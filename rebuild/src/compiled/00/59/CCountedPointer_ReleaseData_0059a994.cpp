#pragma optimize("s",on)
// CCountedPointer<NUISystem::CScrollableList>::ReleaseData() -- drop one reference: at zero the count block's DeleteFunc runs on the
//   data and the block is freed; Data, then Info, cleared
// Leaf: ?ReleaseData@?$CCountedPointer@VCScrollableList@NUISystem@@@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0059A994, 44 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
// NF75 (round 7, land1117fu q10944): T is NUISystem::CScrollableList, not CList. Retail has no identical-code folding, and the
//   byte-identical CCountedPointer<NUISystem::CList> release is 0x00429288 (bound by the landed 0x0041E6D3), so this copy is another
//   instance. Ego_d.pdb's FableLib\debug\ui_frontend_manager.obj contributes exactly CCountedPointer<NUISystem::CScrollableList>::
//   ReleaseData / ShareData / SetStaticCast<CComponent> (ghidra_out/egod_carved_functions.tsv), the TU of CFrontEndManager
//   0x00596CCE / 0x00597006 (the only landed callers) that this COMDAT follows; FableWin.pdb has
//   ?ReleaseData@?$CCountedPointer@VCScrollableList@NUISystem@@@@QAEXXZ (0x0202443B). It is ReleaseData, not the destructor: ShareData
//   0x0059AA86 calls it. Was ??1?$CCountedPointer@VCList@NUISystem@@@@QAE@XZ (a second definition of a CList member).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include "engine/CCountedPointer.h"
namespace NUISystem { class CScrollableList; }
template void CCountedPointer<NUISystem::CScrollableList>::ReleaseData();
