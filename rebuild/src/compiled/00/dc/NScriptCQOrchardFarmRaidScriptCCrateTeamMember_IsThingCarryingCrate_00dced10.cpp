// CCrateTeamMember::IsThingCarryingCrate(const CScriptThing& thing) -- whether the thing carries one of the raid's live crates; every step re-checks IsOutOfAction (an out-of-action member answers false)
// Leaf: ?IsThingCarryingCrate@CCrateTeamMember@CQ_OrchardFarmRaidScript@NScript@@AAE_NABVCScriptThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DCED10, 210 bytes (224 with padding), `ret 4`.
// Callees: CEntityScriptBase::IsActiveThreadTerminating 0x00F35B30 (landed caller 0x00DCEC70; the manifest label is propagated),
//   the crate's CScriptThing virtual slots 75 (+0x12c, alive) and 63 (+0xfc, carried by), the thing's slot 1 (+4).
// Layout: Ego_r.pdb CCrateTeamMember (landed 0x00DCEC70): ParentClass +0x14; the parent script's crate vector of 12-byte
//   CScriptThings at +0x54.
// Shape: the result goes through an inline Return(value) = `if (IsActiveThreadTerminating()) return false; return value;` (the
//   false case keeps the call and drops its result; the true case is `neg ; sbb ; inc`); size() is recomputed per test.
// NF175 (2026-11-20, q6255): re-spelled from the global stand-in CCrateTeamMember (local CScriptThing / CRetailVector / CQ_OrchardFarmRaidScript
//   models) to Ego_r.pdb / FableWin.pdb ?IsThingCarryingCrate@CCrateTeamMember@CQ_OrchardFarmRaidScript@NScript@@AAE_NABVCScriptThing@@@Z (private
//   member of the nested script class) on the shared engine/CQ_OrchardFarmRaidScript.h (CCRATETEAMMEMBER_MAIN opt-in, which declares it) and
//   engine/CScriptThing.h (retail branch: GetName slot 1, IsBeingCarriedBy(const CCharString&) slot 63, IsAlive slot 75), with the landed
//   0x00DCE230 Main define block (Main already binds this spelling at its four calls). The crate vector is the parent's CrateList (+0x54).
//   The stand-in's inline Return(value) helper is written out at each return.
// Inferred: the `if (IsActiveThreadTerminating()) return false;` before each return: retail calls 0x00F35B30 before returning true
//   (`neg ; sbb ; inc` on its result) and before the final false (result dropped); the landed stand-in wrote it as an inline Return(value)
//   helper on its local class, which the shared nested class cannot carry. The original wording (a script macro or helper) is not sourced.
#include "fable_compiler.h"
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CPREDICATEBASE_NOVTABLE
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CQ_ORCHARDFARMRAIDSCRIPT_CCRATETEAMMEMBER_MAIN
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "stlport/fable_containers.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_OrchardFarmRaidScript.h"
#include "engine/CIsThingAlive.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CTimer.h"
#include "engine/fable_script_distance.h"
#include "engine/fable_script_fade.h"

bool NScript::CQ_OrchardFarmRaidScript::CCrateTeamMember::IsThingCarryingCrate(const CScriptThing& thing)
{
    for (unsigned long i = 0; i < ParentClass->CrateList.size(); ++i) {
        if (IsActiveThreadTerminating())
            return false;
        if (ParentClass->CrateList[i].IsAlive()) {
            if (ParentClass->CrateList[i].IsBeingCarriedBy(thing.GetName())) {
                if (IsActiveThreadTerminating())
                    return false;
                return true;
            }
        }
    }
    if (IsActiveThreadTerminating())
        return false;
    return false;
}
