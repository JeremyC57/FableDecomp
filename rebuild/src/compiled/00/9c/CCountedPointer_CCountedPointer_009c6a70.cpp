// CCountedPointer<const CFileInstaller::CFileInfo>::CCountedPointer(const CFileInfo*) -- take ownership of a new file record: a fresh count
// block (count 1, DeleteData 0x009C6640, Data) when non-null
// Leaf: ??0?$CCountedPointer@$$CBVCFileInfo@CFileInstaller@@@@QAE@PBVCFileInfo@CFileInstaller@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009C6A70. Callees: operator new 0x00BFEA1A.
// NF39 (2026-11-16): moved off its address-suffixed stand-in (CCountedPointer_009c6640 with a local count block) onto the shared
//   engine/CCountedPointer.h (the in-class (T*) constructor, out of line as its own COMDAT, `ret 4`; the same source as the landed
//   CCountedPointer<CCreatureActionBase> 0x00695880), T the nested class of engine/CFileInstaller.h: Ego_r.pdb 0x00B149F0 + FableWin.pdb
//   0x0304CAB0 ??0?$CCountedPointer@$$CBVCFileInfo@CFileInstaller@@@@QAE@PBVCFileInfo@CFileInstaller@@@Z; its landed caller AddFile
//   0x009C47A0 already binds this symbol.
#include "engine/CCountedPointer.h"
#include "engine/CFileInstaller.h"

template CCountedPointer<const CFileInstaller::CFileInfo>::CCountedPointer(const CFileInstaller::CFileInfo*);
