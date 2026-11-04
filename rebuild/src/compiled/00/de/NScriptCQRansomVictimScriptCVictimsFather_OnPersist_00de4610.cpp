// NScript::CQ_RansomVictimScript::CVictimsFather::OnPersist(CPersistContext& context) -- saves/loads IntroDone and ReactedToBodyguardsDeath by name (default false)
// Leaf: ?OnPersist@CVictimsFather@CQ_RansomVictimScript@NScript@@EAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DE4610, 67 bytes, thiscall, `ret 4`; calls CPersistContext::Transfer<bool> 0x004045C0 (??$Transfer@_N@CPersistContext@@QAEXPBDAA_NAB_N@Z, already bound).
// Identity (naming wave 35, family op): reg 00de089c "VictimsFather" in 00de0850; Alloc 00de18e0; vtable 012dd190 slot 4; PDB ?OnPersist@CVictimsFather@CQ_RansomVictimScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was _unattributed::PersistTransfer2_00de4610.
//   ?OnPersist@CVictimsFather@CQ_RansomVictimScript@NScript@@EAEXAAVCPersistContext@@@Z in Ego_r.pdb / FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv). The persisted keys are the PDB member names.
// Was: CPersistable_00de4610::PersistTransfer2_00de4610(CPersistContext* context) -- persists the "IntroDone" and "ReactedToBodyguardsDeath" byte flags at +0x1c/+0x1d Retail: Fable.exe (TLC, VC7.1) function @ 0x00DE4610, ora... (catalog stand-in _unattributed).
// Class: engine/CQ_RansomVictimScript.h (shared header: CVictimsFather declares bool IntroDone, bool ReactedToBodyguardsDeath and OnPersist). CPersistContext: engine/CPersistContext.h. Only this member is graded.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#include "engine/CQ_RansomVictimScript.h"
#include "engine/CPersistContext.h"

void NScript::CQ_RansomVictimScript::CVictimsFather::OnPersist(CPersistContext& context)
{
    context.Transfer("IntroDone", IntroDone, false);
    context.Transfer("ReactedToBodyguardsDeath", ReactedToBodyguardsDeath, false);
}
