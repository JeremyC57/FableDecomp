// NScript::CQ_PrisonRaceScript::Main() -- the prison race quest main: bind the six race entity scripts, post them, clear the race-won flag, teleport the hero to the "PrisonRaceStartHSP" marker (its position and heading), wait for the prison courtyard region, then spawn the WatchForHeroWinOrLose thread
// Leaf: ?Main@CQ_PrisonRaceScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DDAE40, 1180 bytes, thiscall, plain `ret`, no EH frame.
// Identity: slot 2 (Main) of the quest-script vtable 0x012DC88C (the landed RegisterMain 0x00DDADA0 spawns it); Ego_r.pdb / Ego_d.pdb
//   ?Main@CQ_PrisonRaceScript@NScript@@UAEXXZ. The six factories are the landed CQ_PrisonRaceScript::C*::Alloc rows (binding name =
//   class name less its `C`); the thread is WatchForHeroWinOrLose 0x00DDB2F0 (Ego_r.pdb ?WatchForHeroWinOrLose@CQ_PrisonRaceScript@NScript@@QAEXXZ).
// Source shape: Ego_d.pdb line table (lines 75..94) and locals (_Fail_Val, RaceStart): one line per binding (78..83, 168 bytes each),
//   `PostAddScriptedEntities();Game->PostAddScriptedEntities();` one line (84), the flag (87), RaceStart (89), the teleport (90), the
//   wait loop with its trailing terminate test on one line (91, 220 bytes), the spawn (93) -- the generated quest-script text of the
//   landed Mains (0x00D27460, 0x00E92A50).
// Callees: operator new 0x00BFEA1A, CCharString(const char*, long) 0x0099EBF0 / copy 0x0099EC30 / ~ 0x0099EAE0,
//   CScriptBase::AddEntityScriptBinding 0x00CB8230 / PostAddScriptedEntities 0x00CB8930 / IsActiveThreadTerminating 0x00CB7940 /
//   AddSpawnedFunction 0x00CB7E50, CSpawnedFuncBase 0x00CDD450 (CSpawnedFunc<CQ_PrisonRaceScript> vtable 0x012DC8A0), ~CScriptThing
//   0x004AA840 (natural exit; expanded at the terminate exits: vtable 0x01238C8C, PImp release, ~CBaseClass 0x0099A2E0);
//   CGameScriptInterfaceBase slots 7 NewScriptFrame, 12 IsRegionLoaded, 64 PostAddScriptedEntities, 70 GetHero, 72
//   GetThingWithScriptName, 473 EntityTeleportToPosition; CScriptThing slots 6 GetPos / 10 GetAngleXY expanded on the exact-type
//   local (NullCoord 0x0143E8E0). Data: CQ_SunnyvaleMasterData::PrisonRaceWonByHero +0xEC.
// Header: engine/CQ_PrisonRaceScript.h opt-ins FABLE_RETAIL_CQ_PRISONRACESCRIPT_WINNINGPOSTGUARD (existing) and
//   FABLE_RETAIL_CQ_PRISONRACESCRIPT_MAIN (new: WatchForHeroWinOrLose and the CPrisonRaceWarden / CPrisonRacer /
//   CPrisonRaceStartingGuard / CWaypointSpeedCheck nested classes, Ego_r.pdb members and sizes).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CQ_PRISONRACESCRIPT_WINNINGPOSTGUARD
#define FABLE_RETAIL_CQ_PRISONRACESCRIPT_MAIN
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/C3DVector.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CQ_PrisonRaceScript.h"
#include "engine/CEntityScriptBinding.h"
#include "engine/CSpawnedFunc.h"
#include "engine/TRetVoid.h"

inline CScriptThing::~CScriptThing() { }
// NF326 one-definition respell: the body in the shape of the self row 0x004AA980 (retail's out-of-line copy), so this TU's pick-any COMDAT equals it.
inline const C3DVector& CScriptThing::GetPos() const { const CScriptThing* imp = PImp.Data; if (imp == 0) return NullCoord; return imp->GetPos(); }
// Bracket: the forwarded call is parenthesised (the landed 0x00DD9520 text, chunk Q13): the PAREN tuple puts the teleport angle's
//   `fstp [esp+18h]` after the GetPos null re-test (`mov ecx,[esp+20h] ; cmp ecx,ebp ; fstp`, 0x00DDB144..0x00DDB14A), as retail;
//   unbracketed (if/return, IsNull()/operator->, conditional-expression bodies) the fstp follows the call at once (1180v1180, one pair).
// NF326 one-definition respell: the self row 0x004AAA40 body with the float result parenthesised as the landed copies spell it (retail's out-of-line copy), so this TU's pick-any COMDAT equals it.
inline float CScriptThing::GetAngleXY() const { const CScriptThing* imp = PImp.Data; if (imp == 0) return 0.0f; return (imp->GetAngleXY()); }

void NScript::CQ_PrisonRaceScript::Main()
{
    TRetVoid _Fail_Val;
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_PrisonRaceScript>("PrisonRaceGuard",this,&CPrisonRaceGuard::Alloc,0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_PrisonRaceScript>("PrisonRaceWinningPostGuard",this,&CPrisonRaceWinningPostGuard::Alloc,0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_PrisonRaceScript>("PrisonRaceWarden",this,&CPrisonRaceWarden::Alloc,0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_PrisonRaceScript>("PrisonRacer",this,&CPrisonRacer::Alloc,0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_PrisonRaceScript>("PrisonRaceStartingGuard",this,&CPrisonRaceStartingGuard::Alloc,0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_PrisonRaceScript>("WaypointSpeedCheck",this,&CWaypointSpeedCheck::Alloc,0));
    PostAddScriptedEntities();Game->PostAddScriptedEntities();
    MasterScriptData->PrisonRaceWonByHero=false;
    CScriptThing RaceStart=Game->GetThingWithScriptName("PrisonRaceStartHSP");
    Game->EntityTeleportToPosition(Game->GetHero(),RaceStart.GetPos(),RaceStart.GetAngleXY(),false,false);
    while(!Game->IsRegionLoaded("PrisonCourtyard")){Game->NewScriptFrame();if(IsActiveThreadTerminating())return _Fail_Val.Return();}if(IsActiveThreadTerminating())return _Fail_Val.Return();
    AddSpawnedFunction(new CSpawnedFunc<CQ_PrisonRaceScript>("WatchForHeroWinOrLose",0,this,&CQ_PrisonRaceScript::WatchForHeroWinOrLose),CCharString(""));
}