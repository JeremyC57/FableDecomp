// NScript::CV_MurderTwistScript::OnPersist(CPersistContext& context) -- saves/loads MurderTwistState by name (default 0L)
// Leaf: ?OnPersist@CV_MurderTwistScript@NScript@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EA8800, 35 bytes, thiscall, `ret 4`; calls CPersistContext::Transfer<long> 0x00410BE0 (??$Transfer@J@CPersistContext@@QAEXPBDAAJABJ@Z, already bound).
// Identity (naming wave 35, family op): outer script vtable 012ee634 slot 4 (slot 1 RegisterMain pushes "Main" 0x012C2F94); enclosing 00ea8830 (slot 2) registers 7 entity scripts whose PDB Alloc outer is CV_MurderTwistScript; PDB ?OnPersist@CV_MurderTwistScript@NScript@@; refs=1; was _unattributed::PersistTransfer_MurderTwistState_00ea8800.
//   ?OnPersist@CV_MurderTwistScript@NScript@@UAEXAAVCPersistContext@@@Z in Ego_r.pdb / FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv). The persisted keys are the PDB member names.
// Was: CPersistable_00ea8800::PersistTransfer_MurderTwistState_00ea8800(CPersistContext* context) -- persists the MurderTwistState dword at +0x4c with a zero default Retail: Fable.exe (TLC, VC7.1) function @ 0x00EA8800, orac... (catalog stand-in _unattributed).
// Class: engine/CV_MurderTwistScript.h (shared header: CV_MurderTwistScript declares long MurderTwistState and OnPersist). CPersistContext: engine/CPersistContext.h. Only this member is graded.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#include "engine/CV_MurderTwistScript.h"
#include "engine/CPersistContext.h"

void NScript::CV_MurderTwistScript::OnPersist(CPersistContext& context)
{
    context.Transfer("MurderTwistState", MurderTwistState, 0L);
}
