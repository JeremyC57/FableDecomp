// CCrateTeamMember::GoOnPatrol() -- an idle member (state 0) picks a patrol spot through the script interface (3 units out when its team is the parent script's current team, 1 unit otherwise, where the move flag is also cleared) unless it is out of action, then moves itself from its team's state-0 counter to state 1; a member already in another state only re-checks that it is out of action
// Leaf: ?GoOnPatrol@CCrateTeamMember@CQ_OrchardFarmRaidScript@NScript@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DCEC70, length 157 bytes (+ int3 padding).
// Callees: CEntityScriptBase::IsActiveThreadTerminating @ 0x00F35B30 (bool: virtual slot 3's thing through 0x00CB7940; the manifest
// "CCreatureAction_TrollWhackGroundBase::Initialise" label is propagated; a `jmp` tail call on the non-idle path), the script
// interface at +4: virtual slot 70 (+0x118: float distance, bool flag -> a position handle), slot 229 (+0x394: a script thing and
// the handle) and slot 238 (+0x3b8: a script thing and 0).
// Shape: the parent's team word (+0x78) compared against TeamID selects the branch; in each branch the nested
// `script->SetPatrolTarget(&m_thing, script->FindPatrolPosition(d, true))` fetches the script's vtable once (reused across the inner
// call, the member reloaded after it); the else branch keeps the thing address in ebx for its third call; then the state
// counters (`dec [team+state*4]`, the state store, `inc [team+4]` with the team reloaded after the store).
// Layout: Ego_r.pdb CCrateTeamMember (60 bytes): vptr +0, the script interface +4 (unnamed), a CScriptThing +8 (unnamed),
// ParentClass +0x14, PMasterData +0x18, CurrentAIState +0x1c, PreviousAIState +0x20, TeamID +0x24, MemberState +0x28, MyTeam +0x2c,
// ThingToPatrolTo +0x30; CCrateTeamManager (landed 0x00CDB360): StateCounter[6] at +0.
// NF175 (2026-11-20, q6254): re-spelled from the global stand-in CCrateTeamMember (?GoOnPatrol@CCrateTeamMember@@QAEXXZ, a local CScriptInterface
//   with descriptive slots) to Ego_r.pdb / FableWin.pdb ?GoOnPatrol@CCrateTeamMember@CQ_OrchardFarmRaidScript@NScript@@AAEXXZ (private member of the
//   nested script class) on the shared engine/CQ_OrchardFarmRaidScript.h (CCRATETEAMMEMBER_MAIN opt-in, which already declares it) with the landed
//   0x00DCE230 Main define block (Main already binds this spelling). The three script slots are CGameScriptInterfaceBase GetHero (70, +0x118),
//   EntityFollowThing (229, +0x394: Me, the hero, distance, true) and EntitySetAsMirroringHeroEnemyRelationsWhileFollowing (238, +0x3b8: Me,
//   false): the old reading of slot 70 as FindPatrolPosition(distance, flag) took the outer call's last two pushes for its own arguments.
//   The team compare is ParentClass->HeroTeam (+0x78).
// Inferred: the discarded `IsActiveThreadTerminating();` on the non-idle path: retail 0x00DCEC7A..0x00DCEC7B `pop esi ; jmp 0x00F35B30`
//   (a tail call whose bool result nothing reads); the landed stand-in had the same statement. Its original wording (a script macro or
//   helper) is not sourced.
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

void NScript::CQ_OrchardFarmRaidScript::CCrateTeamMember::GoOnPatrol()
{
    if (MemberState != 0) {
        IsActiveThreadTerminating();                    // a non-idle member only re-checks itself (the tail call)
        return;
    }
    if (TeamID == ParentClass->HeroTeam) {
        if (IsActiveThreadTerminating())
            return;
        Game->EntityFollowThing(Me, Game->GetHero(), 3.0f, true);
    } else {
        if (IsActiveThreadTerminating())
            return;
        Game->EntityFollowThing(Me, Game->GetHero(), 1.0f, true);
        Game->EntitySetAsMirroringHeroEnemyRelationsWhileFollowing(Me, false);
    }
    --MyTeam->StateCounter[MemberState];
    MemberState = 1;
    ++MyTeam->StateCounter[1];
}
