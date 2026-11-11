// CCountedPointer<CBankFileAsyncEntry>::~CCountedPointer() -- the destructor: ReleaseData() expanded (speed TU)
// Leaf: ??1?$CCountedPointer@VCBankFileAsyncEntry@@@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009C9FC0, 53 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
// Was landed as _global::CCountedPointer_Release_009c9fc0 (stand-in name).
// T = CBankFileAsyncEntry, proven by: CEngineMap::OpenDevelopmentStaticMap 0x00B42530 (/O2) assigns the local to AsyncFileEntry through operator= 0x00A5F6D0 and then releases the local here (Ego_r CEngineMap::AsyncFileEntry is CCountedPointer<CBankFileAsyncEntry>; CEngineLandscapeMap::MapFileEntry the same); the landed caller models CCountedPointer<CBankFileAsyncEntry>; FableWin.pdb ??1 name.
// Member: the destructor. Retail's ~CCountedPointer is `{ ReleaseData(); }`; in a speed TU VC7.1 expands ReleaseData into it
//   (53 bytes, Data zeroed before Info) and /O2 callers (scalar deleting dtors, scope ends past the inline budget) call ??1
//   (probe: /O2 scalar deleting dtor calls ??1, /O1 calls ReleaseData).
// Model: engine/CCountedPointer.h opt-in FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (destructor = ReleaseData(); ReleaseData zeroes Data first).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include "engine/CCountedPointer.h"
class CBankFileAsyncEntry;
template CCountedPointer<CBankFileAsyncEntry>::~CCountedPointer();
