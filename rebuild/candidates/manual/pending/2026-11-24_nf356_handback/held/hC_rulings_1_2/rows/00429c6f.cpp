// CCountedPointer<NUISystem::CComponent>::Reset(NUISystem::CComponent* data) -- ReleaseData(), then adopt data with a fresh count block
// Leaf: ?Reset@?$CCountedPointer@VCComponent@NUISystem@@@@QAEXPAVCComponent@NUISystem@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00429C6F, 59 bytes, `ret 4`. FableWin ?Reset@?$CCountedPointer@VCComponent@NUISystem@@@@QAEXPAVCComponent@NUISystem@@@Z.
//   Replaces the coordinator opt-in FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CALLS_DTOR: the header Reset already calls ReleaseData().
// Identity: Ego_r.pdb ??1?$CCountedPointer@VCComponent@NUISystem@@@@QAE@XZ is a 5-byte function (0x00438DDC, next symbol 0x00438DE1),
//   as retail's 0x00429C53 (`jmp 0x004291DE`), which is called at the scope ends of locals (0x0054522E ProcessEvent, 0x0056BBB9
//   Activate); 0x004291DE (44 bytes) is called on live pointers (ClearBastardChild's loop 0x0041DEED, Reset 0x00429C72, ShareData
//   0x00429217) and is ?ReleaseData@?$CCountedPointer@VCComponent@NUISystem@@@@QAEXXZ (FableWin.pdb; 420 ReleaseData for 419 destructors;
//   Ego_r keeps ReleaseData<CEventBase> out of line, 44 bytes). The same body+jmp pair exists for CAction (0x00429160 / 0x00429B72),
//   CMeshDataBank (0x0042918C / 0x00429BF9) and CList (0x00429288 / 0x00429D8E).
// Model: engine/CCountedPointer.h opt-in FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (~CCountedPointer() { ReleaseData(); }, ReleaseData
//   releases then zeroes Data before Info) and the class from the shared engine/NUISystem_CComponent.h (DeleteData<CComponent>, which Reset
//   stores, needs the complete class with its virtual destructor).
// Port (NF356, 2026-11-24; T3 nf352): was an explicit instantiation of the class template's Reset under a row-head optimize("s", on), on a
//   local NUISystem::CComponent model. The body is now the shared size-mode specialization FABLE_RETAIL_CCOUNTEDPOINTER_CCOMPONENT_RESET_SIZE_BODY
//   (engine/CCountedPointer.h, NF352), which the pick-any copies of 0x00605570 / 0x00626110 also compile. Its block ends with
//   optimize("", on), so this TU sets its pragma AFTER the include (nf189): the ReleaseData / DeleteData instances it emits at the end of the
//   TU stay in size mode. The explicit instantiation below names the specialization, so this object emits it (pick-any). Leaf bytes and
//   relocations are unchanged.
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CCOUNTEDPOINTER_CCOMPONENT_RESET_SIZE_BODY
#include "engine/NUISystem_CComponent.h"
#pragma optimize("s", on)
template void CCountedPointer<NUISystem::CComponent>::Reset(NUISystem::CComponent* data);
