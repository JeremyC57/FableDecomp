// NScript::CQ_TraderEscortScript::CDarkwoodTrader::OnPersist(CPersistContext& context) -- saves/loads BrainState by name (default 0L)
// Leaf: ?OnPersist@CDarkwoodTrader@CQ_TraderEscortScript@NScript@@EAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E05330, 35 bytes, thiscall, `ret 4`; calls CPersistContext::Transfer<long> 0x00410BE0 (??$Transfer@J@CPersistContext@@QAEXPBDAAJABJ@Z, already bound).
// Identity (naming wave 35, family op): reg 00e00dfc "DarkwoodTrader" in 00e00db0; Alloc 00e015f0; vtable 012df424 slot 4; PDB ?OnPersist@CDarkwoodTrader@CQ_TraderEscortScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was _unattributed::PersistTransfer_BrainState_00e05330.
//   ?OnPersist@CDarkwoodTrader@CQ_TraderEscortScript@NScript@@EAEXAAVCPersistContext@@@Z in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv). The persisted keys are the PDB member names.
// Was: CPersistable_00e05330::PersistTransfer_BrainState_00e05330(CPersistContext* context) -- persists the BrainState dword at +0x20 with a zero default Retail: Fable.exe (TLC, VC7.1) function @ 0x00E05330, oracle length 35... (catalog stand-in _unattributed).
// Class: engine/CQ_TraderEscortScript.h (shared header: CDarkwoodTrader declares long BrainState and OnPersist). CPersistContext: engine/CPersistContext.h. Only this member is graded.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#include "engine/CQ_TraderEscortScript.h"
#include "engine/CPersistContext.h"

void NScript::CQ_TraderEscortScript::CDarkwoodTrader::OnPersist(CPersistContext& context)
{
    context.Transfer("BrainState", BrainState, 0L);
}
