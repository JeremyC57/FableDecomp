// CCountedPointer<CLipSyncSampleData>::~CCountedPointer() -- the destructor: ReleaseData() expanded (speed TU)
// Leaf: ??1?$CCountedPointer@VCLipSyncSampleData@@@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E46F0, 53 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
// Was landed as _global::CCountedPointer_Release_006e46f0 (stand-in name).
// T = CLipSyncSampleData, proven by: CLipSyncDataBankEntry::CData's /O2 scalar deleting dtor 0x00A437B0 calls it on +0x1C = LipSyncData (Ego_r CData +0x1C CCountedPointer<CLipSyncSampleData>); CTCTalk::GetCurrentLipSyncFrameData 0x006E2ED0 binds ~CCountedPointer<CLipSyncSampleData>; FableWin.pdb ??1 name.
// Member: the destructor. Retail's ~CCountedPointer is `{ ReleaseData(); }`; in a speed TU VC7.1 expands ReleaseData into it
//   (53 bytes, Data zeroed before Info) and /O2 callers (scalar deleting dtors, scope ends past the inline budget) call ??1
//   (probe: /O2 scalar deleting dtor calls ??1, /O1 calls ReleaseData).
// Model: engine/CCountedPointer.h opt-in FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (destructor = ReleaseData(); ReleaseData zeroes Data first).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include "engine/CCountedPointer.h"
class CLipSyncSampleData;
template CCountedPointer<CLipSyncSampleData>::~CCountedPointer();
