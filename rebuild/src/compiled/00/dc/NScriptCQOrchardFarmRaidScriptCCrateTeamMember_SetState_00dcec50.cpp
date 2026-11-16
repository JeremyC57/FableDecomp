// NScript::CQ_OrchardFarmRaidScript::CCrateTeamMember::SetState(long state) -- move this member from its current state to `state` in its
// team's per-state head count
// Leaf: ?SetState@CCrateTeamMember@CQ_OrchardFarmRaidScript@NScript@@AAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DCEC50, 29 bytes, thiscall, `ret 4`: dec MyTeam->StateCounter[MemberState]; MemberState = state;
//   inc MyTeam->StateCounter[state] (MemberState +0x28, MyTeam +0x2c, StateCounter[6] at +0 of CCrateTeamManager).
// Identity: Ego_r.pdb / FableWin.pdb ?SetState@CCrateTeamMember@CQ_OrchardFarmRaidScript@NScript@@AAEXJ@Z (private; the only
//   CCrateTeamMember member taking one long), engine/CQ_OrchardFarmRaidScript.h; called nine times from CCrateTeamMember::Main 0x00DCE230
//   (landed) with ecx = this. (Landed earlier as a placeholder spelled CHandleBasedMemoryPool::GetPointerFromHandle on a local
//   struct; re-authored 2026-11-16 on the shared header.)
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

namespace NScript {
void CQ_OrchardFarmRaidScript::CCrateTeamMember::SetState(long state)
{
    MyTeam->StateCounter[MemberState]--;
    MemberState = state;
    MyTeam->StateCounter[state]++;
}
}
