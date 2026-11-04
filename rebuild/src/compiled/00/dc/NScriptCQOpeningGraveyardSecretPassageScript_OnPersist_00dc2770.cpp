// NScript::CQ_OpeningGraveyardSecretPassageScript::OnPersist(CPersistContext& context) -- saves/loads QuestStartScreenShown and HaveOpenedDemonDoor by name (default false)
// Leaf: ?OnPersist@CQ_OpeningGraveyardSecretPassageScript@NScript@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DC2770, 67 bytes, thiscall, `ret 4`; calls CPersistContext::Transfer<bool> 0x004045C0 (??$Transfer@_N@CPersistContext@@QAEXPBDAA_NAB_N@Z, already bound).
// Identity (naming wave 35, family op): outer script vtable 012da11c slot 4 (slot 1 RegisterMain pushes "Main" 0x012C2F94); enclosing 00dc27c0 (slot 2) registers 14 entity scripts whose PDB Alloc outer is CQ_OpeningGraveyardSecretPassageScript; PDB ?OnPersist@CQ_OpeningGraveyardSecretPassageScript@NScript@@; refs=1; was _unattributed::PersistTransfer2_00dc2770.
//   ?OnPersist@CQ_OpeningGraveyardSecretPassageScript@NScript@@UAEXAAVCPersistContext@@@Z in Ego_r.pdb / FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv). The persisted keys are the PDB member names.
// Was: CPersistable_00dc2770::PersistTransfer2_00dc2770(CPersistContext* context) -- persists the "QuestStartScreenShown" and "HaveOpenedDemonDoor" byte flags at +0x71/+0x72 Retail: Fable.exe (TLC, VC7.1) function @ 0x00DC27... (catalog stand-in _unattributed).
// Class: engine/CQ_OpeningGraveyardSecretPassageScript.h (shared header: CQ_OpeningGraveyardSecretPassageScript declares bool QuestStartScreenShown, bool HaveOpenedDemonDoor and OnPersist). CPersistContext: engine/CPersistContext.h. Only this member is graded.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#include "engine/CQ_OpeningGraveyardSecretPassageScript.h"
#include "engine/CPersistContext.h"

void NScript::CQ_OpeningGraveyardSecretPassageScript::OnPersist(CPersistContext& context)
{
    context.Transfer("QuestStartScreenShown", QuestStartScreenShown, false);
    context.Transfer("HaveOpenedDemonDoor", HaveOpenedDemonDoor, false);
}
