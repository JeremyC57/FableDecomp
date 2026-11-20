// CCountedPointer<CDiskFileWin32>::~CCountedPointer() -- release one counted pointer
// Leaf: ??1?$CCountedPointer@VCDiskFileWin32@@@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009A9BB0, 53 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
// Naming (NF129, 2026-11-20, q6175): was the extern "C" __fastcall stand-in Dest_val_CCountedPointer over a local RefBlock / CCountedPtr
//   model. T = CDiskFileWin32: FableWin.pdb ??1?$CCountedPointer@VCDiskFileWin32@@@@QAE@XZ; NBankFileManager::OpenRetailBank 0x009A8840
//   destroys its CCountedPointer<CDiskFileWin32> local with it at the version-mismatch exit (0x009A8992), and both landed referencers
//   (0x009A8840, 0x009D06F0) already bind this spelling (exe scan + probe).
// Member: the in-class destructor of engine/CCountedPointer.h (retail emits one ~CCountedPointer COMDAT per T: release, Data = 0, Info = 0).
#include "engine/CCountedPointer.h"
class CDiskFileWin32;
template CCountedPointer<CDiskFileWin32>::~CCountedPointer();
