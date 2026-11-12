// CCountedPointer::Reset(T* data) -- (CCountedPointer<CMovableResourceMemoryPool>) drop the current share (destroying the pointee on the last reference), then adopt data with a fresh count block
// Leaf: ?Reset@?$CCountedPointer@VCMovableResourceMemoryPool@@@@QAEXPAVCMovableResourceMemoryPool@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B3E4A0, 103 bytes, thiscall, `ret 4`. Stores DeleteData 0x00B3E070.
// Names (agentNF27, 2026-11-12, FAM10 NAME_FIXES; was CCountedPointer_00b3e070::Reset, a stand-in with a local count-block model): FableWin.pdb
//   ?Reset@?$CCountedPointer@VCMovableResourceMemoryPool@@@@QAEXPAVCMovableResourceMemoryPool@@@Z; the body stores CCountedPointer<CMovableResourceMemoryPool>::DeleteData 0x00B3E070 (FAM10, row MATCH) as the count block's DeleteFunc.
// Shape: engine/CCountedPointer.h Reset: ReleaseData expanded (its trailing Data store is dead), Data = data, then the count block
//   for a non-null data re-reads Data through `this`; a failed new leaves Info 0.
#include "engine/CCountedPointer.h"
class CMovableResourceMemoryPool;
template void CCountedPointer<CMovableResourceMemoryPool>::Reset(CMovableResourceMemoryPool*);
