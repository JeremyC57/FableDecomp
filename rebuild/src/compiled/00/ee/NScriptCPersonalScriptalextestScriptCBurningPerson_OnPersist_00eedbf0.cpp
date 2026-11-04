// NScript::CPersonalScript_alextestScript::CBurningPerson::OnPersist(CPersistContext& context) -- saves/loads SpeakCounter by name (default 0L)
// Leaf: ?OnPersist@CBurningPerson@CPersonalScript_alextestScript@NScript@@EAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EEDBF0, 35 bytes, thiscall, `ret 4`; calls CPersistContext::Transfer<long> 0x00410BE0 (??$Transfer@J@CPersistContext@@QAEXPBDAAJABJ@Z, already bound).
// Identity (naming wave 35, family op): reg 00eed3a7 "BurningPerson" in 00eed360; Alloc 00eed4d0; vtable 012f5874 slot 4; PDB ?OnPersist@CBurningPerson@CPersonalScript_alextestScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was _unattributed::PersistTransfer_SpeakCounter_00eedbf0.
//   ?OnPersist@CBurningPerson@CPersonalScript_alextestScript@NScript@@EAEXAAVCPersistContext@@@Z in Ego_r.pdb / FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv). The persisted keys are the PDB member names.
// Was: CPersistable_00eedbf0::PersistTransfer_SpeakCounter_00eedbf0(CPersistContext* context) -- persists the SpeakCounter dword at +0x1c with a zero default Retail: Fable.exe (TLC, VC7.1) function @ 0x00EEDBF0, oracle lengt... (catalog stand-in _unattributed).
// Class: engine/CPersonalScript_alextestScript.h (shared header: CBurningPerson declares long SpeakCounter and OnPersist). CPersistContext: engine/CPersistContext.h. Only this member is graded.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#include "engine/CPersonalScript_alextestScript.h"
#include "engine/CPersistContext.h"

void NScript::CPersonalScript_alextestScript::CBurningPerson::OnPersist(CPersistContext& context)
{
    context.Transfer("SpeakCounter", SpeakCounter, 0L);
}
