// CCountedPointer<CDiskFileWin32>::Reset(T* data) -- drop the current share (destroying the pointee on the last reference), then adopt data with a fresh count block
// Leaf: ?Reset@?$CCountedPointer@VCDiskFileWin32@@@@QAEXPAVCDiskFileWin32@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009D1EE0, 103 bytes, thiscall, `ret 4`. Stores DeleteData 0x009A9030.
// Naming (NF129, 2026-11-20, q6177 family): was the stand-in CCountedPointer_009a9030::Reset(void*) over a local SCountBlock model storing
//   Destroy_009a9030. FableWin.pdb ?Reset@?$CCountedPointer@VCDiskFileWin32@@@@QAEXPAVCDiskFileWin32@@@Z; the landed callers
//   CBankFile::BeginUpdateEntries 0x009CE8D0 / OpenFileReadOnly 0x009D1090 / OpenFileReadWrite 0x009D1160 (BankFile +0x7c,
//   CCountedPointer<CDiskFileWin32>) already bind this spelling. The deleter 0x009A9030 is CCountedPointer<CDiskFileWin32>::DeleteData
//   (catalog name; FableWin.pdb ?DeleteData@?$CCountedPointer@VCDiskFileWin32@@@@SIXPAX@Z), as the (T*) ctor 0x009A9B60 stores it.
// Shape: engine/CCountedPointer.h Reset (the 0x00834E40 / 0x009A9C80 recipe).
#include "engine/CCountedPointer.h"
class CDiskFileWin32;
template void CCountedPointer<CDiskFileWin32>::Reset(CDiskFileWin32*);
