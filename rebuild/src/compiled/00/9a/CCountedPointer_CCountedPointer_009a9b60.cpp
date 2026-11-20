// CCountedPointer<CDiskFileWin32>::CCountedPointer(T*) -- take ownership of a new pointee: a fresh count block (count 1, DeleteData, Data) when non-null
// Leaf: ??0?$CCountedPointer@VCDiskFileWin32@@@@QAE@PAVCDiskFileWin32@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009A9B60, 78 bytes, `ret 4`. Callees: operator new 0x00BFEA1A; stores DeleteData 0x009A9030.
// Naming (NF129, 2026-11-20, q6177): was the stand-in CCountedPointer_009a9030 over a local SCountBlock model storing Destroy_009a9030.
//   T = CDiskFileWin32: FableWin.pdb ??0?$CCountedPointer@VCDiskFileWin32@@@@QAE@PAVCDiskFileWin32@@@Z; NBankFileManager::OpenRetailBank
//   `CCountedPointer<CDiskFileWin32> disk_file(new CDiskFileWin32)` (0x009A8890), which binds this spelling. The count block's deleter
//   0x009A9030 is CCountedPointer<CDiskFileWin32>::DeleteData (catalog name; FableWin.pdb ?DeleteData@?$CCountedPointer@VCDiskFileWin32@@@@SIXPAX@Z).
// Member: the in-class (T*) constructor of engine/CCountedPointer.h, emitted out of line as its own COMDAT (the 0x00695880 recipe).
#include "engine/CCountedPointer.h"
class CDiskFileWin32;
template CCountedPointer<CDiskFileWin32>::CCountedPointer(CDiskFileWin32*);
