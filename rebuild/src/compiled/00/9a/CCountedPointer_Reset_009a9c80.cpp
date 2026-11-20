// CCountedPointer<CThreadedFile>::Reset(T* data) -- drop the current share (destroying the pointee on the last reference), then adopt data with a fresh count block
// Leaf: ?Reset@?$CCountedPointer@VCThreadedFile@@@@QAEXPAVCThreadedFile@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009A9C80, 103 bytes, thiscall, `ret 4`. Stores DeleteData 0x009A9040.
// Naming (NF129, 2026-11-20, q6179): was the stand-in CCountedPointer_CThreadedFile::Reset(void*) over a local SCountBlock model storing
//   Destroy_009a9040. FableWin.pdb ?Reset@?$CCountedPointer@VCThreadedFile@@@@QAEXPAVCThreadedFile@@@Z; NBankFileManager::OpenRetailBank
//   `bank->ThreadedFile.Reset(new CThreadedFile)` (0x009A8DFB); the landed 0x009A8840 / 0x009D56C0 / 0x00ADD380 / 0x00ADD4A0 bind this
//   spelling. The deleter 0x009A9040 is CCountedPointer<CThreadedFile>::DeleteData (catalog name).
// Shape: engine/CCountedPointer.h Reset (the 0x00834E40 recipe): ReleaseData expanded (its trailing Data store is dead), Data = data,
//   then the count block for a non-null data re-reads Data through `this`; a failed new leaves Info 0.
#include "engine/CCountedPointer.h"
class CThreadedFile;
template void CCountedPointer<CThreadedFile>::Reset(CThreadedFile*);
