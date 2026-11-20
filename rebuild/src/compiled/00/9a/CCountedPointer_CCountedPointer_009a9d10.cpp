// CCountedPointer<NBankFileManager::CRetailBank>::CCountedPointer(T*) -- take ownership of a new pointee: a fresh count block (count 1, DeleteData, Data) when non-null
// Leaf: ??0?$CCountedPointer@VCRetailBank@NBankFileManager@@@@QAE@PAVCRetailBank@NBankFileManager@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009A9D10, 78 bytes, `ret 4`. Callees: operator new 0x00BFEA1A; stores DeleteData 0x009A90B0.
// Naming (NF129, 2026-11-20, q6178): was the extern "C" __fastcall stand-in CCountedPtr_ctor over a local RefBlock model that stored the
//   deleter as the cast integer 0x009a90b0. T = NBankFileManager::CRetailBank: FableWin.pdb
//   ??0?$CCountedPointer@VCRetailBank@NBankFileManager@@@@QAE@PAVCRetailBank@NBankFileManager@@@Z; NBankFileManager::OpenRetailBank
//   `CCountedPointer<CRetailBank> bank(new CRetailBank)` (0x009A89BE), which binds this spelling. The stored deleter 0x009A90B0 is now
//   the relocation CCountedPointer<NBankFileManager::CRetailBank>::DeleteData (no catalog row; the only referencer is this row).
// Member: the in-class (T*) constructor of engine/CCountedPointer.h, emitted out of line as its own COMDAT (the 0x00695880 recipe).
#include "engine/CCountedPointer.h"
namespace NBankFileManager { class CRetailBank; }
template CCountedPointer<NBankFileManager::CRetailBank>::CCountedPointer(NBankFileManager::CRetailBank*);
