// CCountedPointer<NUISystem::CComponent>::ReleaseData() -- release the shared count (delete the component and the block when it drops to zero), then zero Data and Info
// Leaf: ?ReleaseData@?$CCountedPointer@VCComponent@NUISystem@@@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004291DE, 44 bytes, thiscall `ret`. Was landed as ~CCountedPointer<CComponent> (coordinator fix [1]).
// Identity: Ego_r.pdb ??1?$CCountedPointer@VCComponent@NUISystem@@@@QAE@XZ is a 5-byte function (0x00438DDC, next symbol 0x00438DE1),
//   as retail's 0x00429C53 (`jmp 0x004291DE`), which is called at the scope ends of locals (0x0054522E ProcessEvent, 0x0056BBB9
//   Activate); 0x004291DE (44 bytes) is called on live pointers (ClearBastardChild's loop 0x0041DEED, Reset 0x00429C72, ShareData
//   0x00429217) and is ?ReleaseData@?$CCountedPointer@VCComponent@NUISystem@@@@QAEXXZ (FableWin.pdb; 420 ReleaseData for 419 destructors;
//   Ego_r keeps ReleaseData<CEventBase> out of line, 44 bytes). The same body+jmp pair exists for CAction (0x00429160 / 0x00429B72),
//   CMeshDataBank (0x0042918C / 0x00429BF9) and CList (0x00429288 / 0x00429D8E).
// Model: engine/CCountedPointer.h opt-in FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (ReleaseData releases, then zeroes Data before Info).
// Port (NF356, 2026-11-24; T3 nf352): was an explicit instantiation of the class template's ReleaseData under a row-head
//   optimize("s",on), on a local NUISystem::CComponent model. The body is now the shared size-mode specialization
//   FABLE_RETAIL_CCOUNTEDPOINTER_CCOMPONENT_RELEASEDATA_SIZE_BODY (engine/CCountedPointer.h, NF352), which the pick-any copies of 0x00605570 /
//   0x00626110 also compile; it never touches the pointee, so the header's forward declaration of NUISystem::CComponent is enough (no class
//   model here). Its block ends with optimize("", on), so this TU sets its pragma AFTER the include (nf189). The explicit instantiation
//   below names the specialization, so this object emits it (pick-any). Leaf bytes and relocations are unchanged.
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CCOUNTEDPOINTER_CCOMPONENT_RELEASEDATA_SIZE_BODY
#include "engine/CCountedPointer.h"
#pragma optimize("s", on)
template void CCountedPointer<NUISystem::CComponent>::ReleaseData();
