#pragma optimize("s",on)
// CCountedPointer<NUISystem::CText>::ReleaseData() -- release the shared count (delete the text and the block when it drops to zero), then zero Data and Info
// Leaf: ?ReleaseData@?$CCountedPointer@VCText@NUISystem@@@@QAEXXZ
//
// NF39 (2026-11-16): renamed from ~CCountedPointer<NUISystem::CText> onto FableWin.pdb ?ReleaseData@?$CCountedPointer@VCText@NUISystem@@@@QAEXXZ
//   (ghidra_out/labels_agentNF_2026-11-10.tsv): the 44-byte release block of the NUISystem TU with no `jmp` destructor twin, the same pair
//   as the landed ReleaseData<CComponent> 0x004291DE (engine/CCountedPointer.h FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA: Data zeroed before
//   Info). 20 of its 21 landed callers already bind this symbol (probe); the 21st, ShareData<CText> 0x00429460, is respelled with it.
// Model: the same source as 0x004291DE with T = NUISystem::CText (catalog T + FableWin ??1 / ReleaseData symbols).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include "engine/CCountedPointer.h"
namespace NUISystem { class CText; }
template void CCountedPointer<NUISystem::CText>::ReleaseData();
