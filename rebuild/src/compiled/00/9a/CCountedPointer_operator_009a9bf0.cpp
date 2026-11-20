// CCountedPointer<CDiskFileWin32>::operator=(const CCountedPointer&) -- share the other pointer: ShareData(Data, Info), return *this
// Leaf: ??4?$CCountedPointer@VCDiskFileWin32@@@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009A9BF0, 72 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
// Naming (NF129, 2026-11-20, q6176): was the extern "C" __fastcall stand-in CCountedPtr_assign over a local RefBlock / CCountedPtr model.
//   T = CDiskFileWin32: Ego_r.pdb + FableWin.pdb ??4?$CCountedPointer@VCDiskFileWin32@@@@QAEAAV0@ABV0@@Z; NBankFileManager::OpenRetailBank
//   `bank->DiskFile = disk_file` (0x009A89FA); both landed referencers (0x009A8840, 0x009D06F0) already bind this spelling (exe scan + probe).
// Member: the in-class operator= of engine/CCountedPointer.h, the 0x009D6FD0 CCountedPointer<CThreadedFile> recipe.
// Model: opt-in FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (ReleaseData zeroes Data first; its stores die under ShareData's own).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include "engine/CCountedPointer.h"
class CDiskFileWin32;
template CCountedPointer<CDiskFileWin32>& CCountedPointer<CDiskFileWin32>::operator=(const CCountedPointer<CDiskFileWin32>&);
