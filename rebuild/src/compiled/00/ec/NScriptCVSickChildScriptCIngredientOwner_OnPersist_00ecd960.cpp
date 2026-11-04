// NScript::CV_SickChildScript::CIngredientOwner::OnPersist(CPersistContext& context) -- saves/loads BoughtSpecialStuff and StoleSpecialStuff by name (default false)
// Leaf: ?OnPersist@CIngredientOwner@CV_SickChildScript@NScript@@EAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ECD960, 67 bytes, thiscall, `ret 4`; calls CPersistContext::Transfer<bool> 0x004045C0 (??$Transfer@_N@CPersistContext@@QAEXPBDAA_NAB_N@Z, already bound).
// Identity (naming wave 35, family op): reg 00ec5781 "IngredientOwner" in 00ec54e0; Alloc 00ecafa0; vtable 012f2000 slot 4; PDB ?OnPersist@CIngredientOwner@CV_SickChildScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was _unattributed::PersistTransfer2_00ecd960.
//   ?OnPersist@CIngredientOwner@CV_SickChildScript@NScript@@EAEXAAVCPersistContext@@@Z in Ego_r.pdb / FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv). The persisted keys are the PDB member names.
// Was: CPersistable_00ecd960::PersistTransfer2_00ecd960(CPersistContext* context) -- persists the "BoughtSpecialStuff" and "StoleSpecialStuff" byte flags at +0x1c/+0x1d Retail: Fable.exe (TLC, VC7.1) function @ 0x00ECD960, o... (catalog stand-in _unattributed).
// Class: engine/CV_SickChildScript.h (shared header: CIngredientOwner declares bool BoughtSpecialStuff, bool StoleSpecialStuff and OnPersist). CPersistContext: engine/CPersistContext.h. Only this member is graded.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRING_OUTOFLINE_CONVERSION
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_NSCRIPT
#include "engine/CV_SickChildScript.h"
#include "engine/CPersistContext.h"

void NScript::CV_SickChildScript::CIngredientOwner::OnPersist(CPersistContext& context)
{
    context.Transfer("BoughtSpecialStuff", BoughtSpecialStuff, false);
    context.Transfer("StoleSpecialStuff", StoleSpecialStuff, false);
}
