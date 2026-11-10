// CCountedPointer<NUISystem::CComponent>::ShareData(NUISystem::CComponent* data, CCPPointerInfo* info) -- share another pointer's count block
// Leaf: ?ShareData@?$CCountedPointer@VCComponent@NUISystem@@@@QAEXPAVCComponent@NUISystem@@PAVCCPPointerInfo@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042920A, 38 bytes, `ret 8`; called by operator= 0x00429C58. PDB name sets: ShareData is a CCountedPointer member
//   (188 instantiations, e.g. ?ShareData@?$CCountedPointer@VCAction@NUISystem@@@@..); T = CComponent from the release it calls. Was a descriptive CCPtr::ShareData.
// Identity: Ego_r.pdb ??1?$CCountedPointer@VCComponent@NUISystem@@@@QAE@XZ is a 5-byte function (0x00438DDC, next symbol 0x00438DE1),
//   as retail's 0x00429C53 (`jmp 0x004291DE`), which is called at the scope ends of locals (0x0054522E ProcessEvent, 0x0056BBB9
//   Activate); 0x004291DE (44 bytes) is called on live pointers (ClearBastardChild's loop 0x0041DEED, Reset 0x00429C72, ShareData
//   0x00429217) and is ?ReleaseData@?$CCountedPointer@VCComponent@NUISystem@@@@QAEXXZ (FableWin.pdb; 420 ReleaseData for 419 destructors;
//   Ego_r keeps ReleaseData<CEventBase> out of line, 44 bytes). The same body+jmp pair exists for CAction (0x00429160 / 0x00429B72),
//   CMeshDataBank (0x0042918C / 0x00429BF9) and CList (0x00429288 / 0x00429D8E).
// Model: engine/CCountedPointer.h opt-in FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (~CCountedPointer() { ReleaseData(); }, ReleaseData
//   releases then zeroes Data before Info); in this size-optimised TU VC7.1 keeps the 44-byte ReleaseData out of line by itself.
#pragma optimize("s",on)
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include "engine/CCountedPointer.h"
namespace NUISystem { class CComponent { public: virtual ~CComponent(); }; }
template void CCountedPointer<NUISystem::CComponent>::ShareData(NUISystem::CComponent* data, CCPPointerInfo* info);
