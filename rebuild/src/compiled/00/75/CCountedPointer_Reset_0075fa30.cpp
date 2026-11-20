// CCountedPointer::Reset(T* data) -- (CCountedPointer<CCardinalSpline>) drop the current share (destroying the pointee on the last reference), then adopt data with a fresh count block
// Leaf: ?Reset@?$CCountedPointer@VCCardinalSpline@@@@QAEXPAVCCardinalSpline@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0075FA30, 103 bytes, thiscall, `ret 4`. Stores DeleteData 0x0075F830.
// Names (NF120, PRIORITY q4803; was the generated stand-in CCountedPointer_0075f830::Reset over a local count-block model): FableWin.pdb
//   ?Reset@?$CCountedPointer@VCCardinalSpline@@@@QAEXPAVCCardinalSpline@@@Z; the body stores the catalog's CCountedPointer<CCardinalSpline>::DeleteData
//   0x0075F830 as the count block's DeleteFunc, and the landed 0x00B149D0 / 0x00B13530 call this address for Spline.Reset(new CCardinalSpline)
//   with this spelling (RELOCATION_MATCH).
// Shape: engine/CCountedPointer.h Reset (the 0x00834E40 recipe): ReleaseData expanded (its trailing Data store is dead), Data = data, then
//   the count block for a non-null data re-reads Data through `this`; a failed new leaves Info 0.
#include "engine/CCountedPointer.h"
class CCardinalSpline;
template void CCountedPointer<CCardinalSpline>::Reset(CCardinalSpline*);
