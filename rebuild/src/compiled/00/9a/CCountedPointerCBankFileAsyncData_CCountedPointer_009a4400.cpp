// CCountedPointer<CBankFileAsyncData>::~CCountedPointer() -- the destructor: ReleaseData() expanded (speed TU)
// Leaf: ??1?$CCountedPointer@VCBankFileAsyncData@@@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009A4400, 53 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
// Was landed as _global::CCountedPointer_Release_009a4400 (stand-in name).
// T = CBankFileAsyncData, proven by: /O2 scalar deleting dtors of CLipSyncDataBankEntry::CData 0x00A437B0 (+0x24) and CEmitterData 0x00AF1BD0 (+0x1C) call it: both offsets are FileData, CCountedPointer<CBankFileAsyncData> (Ego_r CData +0x24, CEmitterData +0x1C); the adjacent operator= 0x009A4440 binds CCountedPointer<CBankFileAsyncData> (0x00AF1360); FableWin.pdb ??1 name.
// Member: the destructor. Retail's ~CCountedPointer is `{ ReleaseData(); }`; in a speed TU VC7.1 expands ReleaseData into it
//   (53 bytes, Data zeroed before Info) and /O2 callers (scalar deleting dtors, scope ends past the inline budget) call ??1
//   (probe: /O2 scalar deleting dtor calls ??1, /O1 calls ReleaseData).
// Model: engine/CCountedPointer.h opt-in FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (destructor = ReleaseData(); ReleaseData zeroes Data first).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include "engine/CCountedPointer.h"
class CBankFileAsyncData;
template CCountedPointer<CBankFileAsyncData>::~CCountedPointer();
