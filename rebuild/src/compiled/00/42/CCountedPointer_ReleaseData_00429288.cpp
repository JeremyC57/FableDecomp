#pragma optimize("s",on)
// CCountedPointer<NUISystem::CList>::ReleaseData() -- drop one reference: at zero the count block's DeleteFunc runs on the data and the
//   block is freed; Data, then Info, cleared
// Leaf: ?ReleaseData@?$CCountedPointer@VCList@NUISystem@@@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00429288, 44 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
// NF75 (round 7, land1117fu q10944): ReleaseData, not the destructor: CCountedPointer<CList>::ShareData 0x00429486 calls it, the landed
//   NUISystem::CManager::Action 0x0041E6D3 binds ?ReleaseData@?$CCountedPointer@VCList@NUISystem@@@@QAEXXZ, and FableWin.pdb has that
//   symbol (in the NUISystem TUs the destructor `{ ReleaseData(); }` is a separate jmp COMDAT, as for CComponent 0x004291DE / 0x00429C53,
//   engine/CCountedPointer.h). The byte-identical 0x0059A994 is the CScrollableList instance. Was ??1?$CCountedPointer@VCList@NUISystem@@@@QAE@XZ.
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include "engine/CCountedPointer.h"
namespace NUISystem { class CList; }
template void CCountedPointer<NUISystem::CList>::ReleaseData();
