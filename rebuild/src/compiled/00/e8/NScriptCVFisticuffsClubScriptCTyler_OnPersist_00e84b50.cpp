// NScript::CV_FisticuffsClubScript::CTyler::OnPersist(CPersistContext& context) -- saves/loads IntroducedPunchClub by name (default false)
// Leaf: ?OnPersist@CTyler@CV_FisticuffsClubScript@NScript@@EAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E84B50, 33 bytes, thiscall, `ret 4`; calls CPersistContext::Transfer<bool> 0x004045C0 (??$Transfer@_N@CPersistContext@@QAEXPBDAA_NAB_N@Z, already bound).
// Identity (naming wave 35, family op): reg 00e82338 "Tyler" in 00e82220; Alloc 00e83570; vtable 012eb0c0 slot 4; PDB ?OnPersist@CTyler@CV_FisticuffsClubScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was _unattributed::PersistTransfer_IntroducedPunchClub_00e84b50.
//   ?OnPersist@CTyler@CV_FisticuffsClubScript@NScript@@EAEXAAVCPersistContext@@@Z in Ego_r.pdb / FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv). The persisted keys are the PDB member names.
// Was: CPersistable_00e84b50::PersistTransfer_IntroducedPunchClub_00e84b50(CPersistContext* context) -- persists the "IntroducedPunchClub" flag at +0x1e Retail: Fable.exe (TLC, VC7.1) function @ 0x00E84B50, oracle length 33 ... (catalog stand-in _unattributed).
// Class: engine/NScript_CV_FisticuffsClubScript.h (shared header: CTyler declares bool IntroducedPunchClub and OnPersist); includes as the landed includer rebuild/src/compiled/00/e8/NScriptCVFisticuffsClubScriptCTyler_FightWarmupRounds_00e8a2b0.cpp. CPersistContext: engine/CPersistContext.h. Only this member is graded.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_PDCB_TYPED_MEMBERS
#include <string.h>
#include "engine/EOpinionDeedType.h"
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_STLPORT_HTTP
#define FABLE_STLPORT_CONSTRUCTORS
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/C3DVector.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CRGBColour.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CSpawnedFunc.h"
#include "stlport/fable_map.h"
#include "engine/NScript_CV_FisticuffsClubScript.h"
#include "engine/CPersistContext.h"

void NScript::CV_FisticuffsClubScript::CTyler::OnPersist(CPersistContext& context)
{
    context.Transfer("IntroducedPunchClub", IntroducedPunchClub, false);
}
