// NScript::CV_LostTraderScript::OnPersist(CPersistContext& context) -- saves/loads g_bTraderHit by name (default false)
// Leaf: ?OnPersist@CV_LostTraderScript@NScript@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EA5930, 33 bytes, thiscall, `ret 4`; calls CPersistContext::Transfer<bool> 0x004045C0 (??$Transfer@_N@CPersistContext@@QAEXPBDAA_NAB_N@Z, already bound).
// Identity (naming wave 35, family op): outer script vtable 012edce8 slot 4 (slot 1 RegisterMain pushes "Main" 0x012C2F94); enclosing 00ea2e80 (slot 2) registers 3 entity scripts whose PDB Alloc outer is CV_LostTraderScript; PDB ?OnPersist@CV_LostTraderScript@NScript@@; refs=1; was _unattributed::PersistTransfer_gbTraderHit_00ea5930.
//   ?OnPersist@CV_LostTraderScript@NScript@@UAEXAAVCPersistContext@@@Z in Ego_r.pdb / FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv). The persisted keys are the PDB member names.
// Was: CPersistable_00ea5930::PersistTransfer_gbTraderHit_00ea5930(CPersistContext* context) -- persists the "g_bTraderHit" flag at +0x4b Retail: Fable.exe (TLC, VC7.1) function @ 0x00EA5930, oracle length 33 bytes. (catalog stand-in _unattributed).
// Class: engine/CV_LostTraderScript.h (shared header: CV_LostTraderScript declares bool g_bTraderHit and OnPersist). CPersistContext: engine/CPersistContext.h. Only this member is graded.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CENTITYSCRIPTBINDING_FORCEINLINE_CTOR
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CV_LostTraderScript.h"
#include "engine/CPersistContext.h"

void NScript::CV_LostTraderScript::OnPersist(CPersistContext& context)
{
    context.Transfer("g_bTraderHit", g_bTraderHit, false);
}
