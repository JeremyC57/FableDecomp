// NScript::COracleMinigameTestScript::COracleGamesmaster::OnPersist(CPersistContext& context) -- saves/loads SaidIntro by name (default false)
// Leaf: ?OnPersist@COracleGamesmaster@COracleMinigameTestScript@NScript@@EAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F01300, 33 bytes, thiscall, `ret 4`; calls CPersistContext::Transfer<bool> 0x004045C0 (??$Transfer@_N@CPersistContext@@QAEXPBDAA_NAB_N@Z, already bound).
// Identity (naming wave 35, family op): reg 00effe57 "OracleGamesmaster" in 00effe10; Alloc 00f00000; vtable 012f7100 slot 4; PDB ?OnPersist@COracleGamesmaster@COracleMinigameTestScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was _unattributed::PersistTransfer_SaidIntro_00f01300.
//   ?OnPersist@COracleGamesmaster@COracleMinigameTestScript@NScript@@EAEXAAVCPersistContext@@@Z in Ego_r.pdb / FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv). The persisted keys are the PDB member names.
// Was: CPersistable_00f01300::PersistTransfer_SaidIntro_00f01300(CPersistContext* context) -- persists the "SaidIntro" flag at +0x1c Retail: Fable.exe (TLC, VC7.1) function @ 0x00F01300, oracle length 33 bytes. (catalog stand-in _unattributed).
// Class: engine/COracleMinigameTestScript.h (shared header: COracleGamesmaster declares bool SaidIntro and OnPersist). CPersistContext: engine/CPersistContext.h. Only this member is graded.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_NSCRIPT
#include "engine/COracleMinigameTestScript.h"
#include "engine/CPersistContext.h"

void NScript::COracleMinigameTestScript::COracleGamesmaster::OnPersist(CPersistContext& context)
{
    context.Transfer("SaidIntro", SaidIntro, false);
}
