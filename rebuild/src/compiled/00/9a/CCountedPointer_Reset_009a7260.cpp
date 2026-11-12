// CCountedPointer::Reset(T* data) -- (CCountedPointer<CMeshBankManager>) drop the current share (destroying the pointee on the last reference), then adopt data with a fresh count block
// Leaf: ?Reset@?$CCountedPointer@VCMeshBankManager@@@@QAEXPAVCMeshBankManager@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009A7260, 103 bytes, thiscall, `ret 4`. Stores DeleteData 0x009A6BF0.
// Names (agentNF27, 2026-11-12, FAM10 NAME_FIXES; was CCountedPointer_009a6bf0::Reset, a stand-in with a local count-block model): FableWin.pdb
//   ?Reset@?$CCountedPointer@VCMeshBankManager@@@@QAEXPAVCMeshBankManager@@@Z; the body stores CCountedPointer<CMeshBankManager>::DeleteData 0x009A6BF0 (FAM10, row MATCH) as the count block's DeleteFunc.
// Shape: engine/CCountedPointer.h Reset: ReleaseData expanded (its trailing Data store is dead), Data = data, then the count block
//   for a non-null data re-reads Data through `this`; a failed new leaves Info 0.
#include "engine/CCountedPointer.h"
class CMeshBankManager;
template void CCountedPointer<CMeshBankManager>::Reset(CMeshBankManager*);
