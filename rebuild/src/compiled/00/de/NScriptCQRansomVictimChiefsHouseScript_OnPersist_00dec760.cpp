// NScript::CQ_RansomVictimChiefsHouseScript::OnPersist(CPersistContext& context) -- saves/loads SetChiefsHut by name (default false)
// Leaf: ?OnPersist@CQ_RansomVictimChiefsHouseScript@NScript@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DEC760, 33 bytes, thiscall, `ret 4`; calls CPersistContext::Transfer<bool> 0x004045C0 (??$Transfer@_N@CPersistContext@@QAEXPBDAA_NAB_N@Z, already bound).
// Identity (naming wave 35, family op): outer script vtable 012ddba8 slot 4 (slot 1 RegisterMain pushes "Main" 0x012C2F94); enclosing 00debef0 (slot 2) registers 1 entity scripts whose PDB Alloc outer is CQ_RansomVictimChiefsHouseScript; PDB ?OnPersist@CQ_RansomVictimChiefsHouseScript@NScript@@; refs=1; was _unattributed::PersistTransfer_SetChiefsHut_00dec760.
//   ?OnPersist@CQ_RansomVictimChiefsHouseScript@NScript@@UAEXAAVCPersistContext@@@Z in Ego_r.pdb / FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv). The persisted keys are the PDB member names.
// Was: CPersistable_00dec760::PersistTransfer_SetChiefsHut_00dec760(CPersistContext* context) -- persists the "SetChiefsHut" flag at +0x48 Retail: Fable.exe (TLC, VC7.1) function @ 0x00DEC760, oracle length 33 bytes. (catalog stand-in _unattributed).
// Class: engine/CQ_RansomVictimChiefsHouseScript.h (shared header: CQ_RansomVictimChiefsHouseScript declares bool SetChiefsHut and OnPersist). CPersistContext: engine/CPersistContext.h. Only this member is graded.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_NSCRIPT
#include "engine/CQ_RansomVictimChiefsHouseScript.h"
#include "engine/CPersistContext.h"

void NScript::CQ_RansomVictimChiefsHouseScript::OnPersist(CPersistContext& context)
{
    context.Transfer("SetChiefsHut", SetChiefsHut, false);
}
