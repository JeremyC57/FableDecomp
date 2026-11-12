// CCountedPointer::Reset(T* data) -- (CCountedPointer<CFontManager>) drop the current share (destroying the pointee on the last reference), then adopt data with a fresh count block
// Leaf: ?Reset@?$CCountedPointer@VCFontManager@@@@QAEXPAVCFontManager@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009A7340, 103 bytes, thiscall, `ret 4`. Stores DeleteData 0x009A6C10.
// Names (agentNF27, 2026-11-12, FAM10 NAME_FIXES; was CCountedPointer_009a6c10::Reset, a stand-in with a local count-block model): FableWin.pdb
//   ?Reset@?$CCountedPointer@VCFontManager@@@@QAEXPAVCFontManager@@@Z; the body stores CCountedPointer<CFontManager>::DeleteData 0x009A6C10 (FAM10, row MATCH) as the count block's DeleteFunc.
// Shape: engine/CCountedPointer.h Reset: ReleaseData expanded (its trailing Data store is dead), Data = data, then the count block
//   for a non-null data re-reads Data through `this`; a failed new leaves Info 0.
#include "engine/CCountedPointer.h"
class CFontManager;
template void CCountedPointer<CFontManager>::Reset(CFontManager*);
