// CCountedPointer::Reset(T* data) -- (CCountedPointer<C3DMeshStripifier>) drop the current share (destroying the pointee on the last reference), then adopt data with a fresh count block
// Leaf: ?Reset@?$CCountedPointer@VC3DMeshStripifier@@@@QAEXPAVC3DMeshStripifier@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A29FC0, 103 bytes, thiscall, `ret 4`. Stores DeleteData 0x00A28520.
// Names (agentNF27, 2026-11-12, FAM10 NAME_FIXES; was CCountedPointer_00a28520::Reset, a stand-in with a local count-block model): FableWin.pdb
//   ?Reset@?$CCountedPointer@VC3DMeshStripifier@@@@QAEXPAVC3DMeshStripifier@@@Z; the body stores CCountedPointer<C3DMeshStripifier>::DeleteData 0x00A28520 (FAM10, row MATCH) as the count block's DeleteFunc.
// Shape: engine/CCountedPointer.h Reset: ReleaseData expanded (its trailing Data store is dead), Data = data, then the count block
//   for a non-null data re-reads Data through `this`; a failed new leaves Info 0.
#include "engine/CCountedPointer.h"
class C3DMeshStripifier;
template void CCountedPointer<C3DMeshStripifier>::Reset(C3DMeshStripifier*);
