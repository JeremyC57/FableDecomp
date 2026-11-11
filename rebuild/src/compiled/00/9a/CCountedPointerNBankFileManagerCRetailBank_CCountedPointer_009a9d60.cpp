// CCountedPointer::~CCountedPointer() -- the destructor: ReleaseData() expanded (speed TU) (T = NBankFileManager::CRetailBank)
// Leaf: ??1?$CCountedPointer@VCRetailBank@NBankFileManager@@@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009A9D60, 53 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
// Was landed as _global::CCountedPointer_Release_009a9d60 (stand-in name).
// T = NBankFileManager::CRetailBank, proven by: CBankFile::OpenReadOnly 0x009D06F0 binds ~CCountedPointer<NBankFileManager::CRetailBank>, CBankFileAsync::OpenReadOnly 0x009D56C0 (/O2) calls it at scope end; Ego_r has list<CCountedPointer<NBankFileManager::CRetailBank>>; FableWin.pdb ??1 name.
// Member: the destructor. Retail's ~CCountedPointer is `{ ReleaseData(); }`; in a speed TU VC7.1 expands ReleaseData into it
//   (53 bytes, Data zeroed before Info) and /O2 callers (scalar deleting dtors, scope ends past the inline budget) call ??1
//   (probe: /O2 scalar deleting dtor calls ??1, /O1 calls ReleaseData).
// Model: engine/CCountedPointer.h opt-in FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (destructor = ReleaseData(); ReleaseData zeroes Data first).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include "engine/CCountedPointer.h"
namespace NBankFileManager { class CRetailBank; }
template CCountedPointer<NBankFileManager::CRetailBank>::~CCountedPointer();
