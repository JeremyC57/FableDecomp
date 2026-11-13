// NScript::CQ_OrchardFarmRaidScript::CTeamSpawn::Main() -- Orchard Farm raid team spawner: wait for the introduction, start the team's reinforcements timer (and, for the hero's bandit team, show the bandits-left counter); then until Whisper spawns, each frame: the guard team (TeamID 0) refills its members up to TeamMemberLimit at Me (hero more than 15 away and Me off camera) or at a random "EitherTeamSpawn" marker, every TeamRespawnTime; the bandit team updates the counter and, when its members are all dead, waits BanditReinforcementDelay, counts a wave (a guard near the hero says NEXT_WAVE; after three waves the hero's team has failed and the spawner stops) and spawns the next wave at Me (the second member an archer when the hero leads the guards); finally remove the counter
// Leaf: ?Main@CTeamSpawn@CQ_OrchardFarmRaidScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DCD350, 1833 bytes (`sub esp,74h` .. the last of five epilogue copies `ret` at 0x00DCDA78;
//   the next function, MakeTeamMemberComment, starts at 0x00DCDA80), thiscall, frame 0x74, no EH frame.
// Identity (two sources): Ego_r.pdb / FableWin.pdb ?Main@CTeamSpawn@CQ_OrchardFarmRaidScript@NScript@@EAEXXZ (private virtual); slot 1
//   of the CTeamSpawn entity vtable 0x012DB2D4 (slot 0 ??_G 0x00DCF6E0, slot 2 Init 0x00DCD1D0, slot 3 GetParentScript 0x00DCD1C0),
//   the vtable the landed ctor 0x00DCD160 stores; the members read are Ego_r.pdb CTeamSpawn's (TeamID +0x28 .. BanditsLeftID +0x3c).
// Callees: CEntityScriptBase::IsActiveThreadTerminating 0x00F35B30; pDSTGame 0x0143E8F8 slots 89 SetTimer / 90 GetTimer (the inline
//   CTimer::operator=(long) / GetTime of CCrateTeamManager::TeamReinforcementsTimer); Game slots 7 NewScriptFrame, 70 GetHero, 74
//   GetRandomThingWithScriptName, 77 GetNearestWithScriptName, 91 CreateCreature, 321 DisplayQuestInfo, 329 AddQuestInfoCounterList,
//   336 UpdateQuestInfoCounterList, 338 RemoveQuestInfoElement, 376 Pause, 421 IsCameraPosOnScreen, 465 EntityAttachToScript;
//   CScriptThing slot 6 GetPos (virtual), IsAlive 0x004AB130 (devirtualised on the local), CScriptThing() 0x006E7B40, operator=
//   0x008AB980, ~CScriptThing 0x004AA840; IsDistanceBetweenThingsOver 0x00CBE3EA / IsDistanceBetweenThingsUnder 0x00CBE2FF
//   (__fastcall); CQ_OrchardFarmRaidScript::MakeTeamMemberComment 0x00DCDA80 (Ego_r.pdb / FableWin.pdb spelling: CScriptThing& and
//   the ERaidComment enum); CCharString 0x0099EBF0 / ~ 0x0099EAE0.
// Data: pGlobalScriptDef 0x0143E90C BanditReinforcementDelay +0xd74 (engine/CScriptDef.h); literals "HUD_QUEST_ICON_BANDIT"
//   0x012C88C8, "Q_OrchardFarmRaid" 0x012C32DC, "EitherTeamSpawn" 0x012DB30C, "GuardTeamMember" 0x012DB27C, "NEXT_WAVE" 0x012DB33C,
//   "CREATURE_BANDIT_ARCHER_LEVEL1" 0x012DB31C. Layout: engine/CQ_OrchardFarmRaidScript.h (new, hdr overlay).
// Shape: the generated-script form (TRetVoid _Fail_Val; a terminate check at the head of every block and after every loop). The two
//   `if (TeamID == ParentClass->HeroTeam) { check } else { check }` statements whose arms hold only the terminate check: c2 merges the
//   two arms and drops the branch, leaving the dead HeroTeam / TeamID loads (0x00DCD4C5) and the dead `cmp edx,ecx` (0x00DCD88A) of
//   retail. Each spawned member of the guard loops is copy-initialised from CreateCreature (built in place); the bandit loop's member
//   is default-constructed and assigned in the if / else (CScriptThing() 0x006E7B40 + operator= + temporary). The member count read
//   once into a named local (ebx across each loop); string literals passed straight to `const CCharString&` parameters.
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CQ_ORCHARDFARMRAIDSCRIPT_CTEAMSPAWN_MAIN
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "stlport/fable_containers.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_OrchardFarmRaidScript.h"
#include "engine/CTimer.h"
#include "engine/CScriptDef.h"
#include "engine/CDefPointer.h"
#include "engine/fable_script_distance.h"
#include "engine/TRetVoid.h"
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8
extern CDefPointer<const CScriptDef> pGlobalScriptDef;   // 0x0143E90C
FABLE_FORCEINLINE CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
FABLE_FORCEINLINE long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }

void NScript::CQ_OrchardFarmRaidScript::CTeamSpawn::Main()
{
    TRetVoid _Fail_Val;
    while (!ParentClass->DoneIntroduction) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    ParentClass->Teams[TeamID].TeamReinforcementsTimer = TeamRespawnTime;
    if (TeamID == 1 && ParentClass->HeroTeam == 1) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        Game->DisplayQuestInfo(true);
        BanditsLeftID = Game->AddQuestInfoCounterList("HUD_QUEST_ICON_BANDIT", TeamMemberLimit * 3, 1.0f);
        Game->UpdateQuestInfoCounterList(BanditsLeftID, TeamMemberLimit * 3, -1);
    }
    while (!ParentClass->WhisperSpawned) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        if (TeamID == 0) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            if (ParentClass->Teams[TeamID].TeamReinforcementsTimer.GetTime() == 0 && ParentClass->Teams[TeamID].MemberCount < TeamMemberLimit) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                if (TeamID == ParentClass->HeroTeam) {
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                } else {
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
                ParentClass->Teams[TeamID].TeamReinforcementsTimer = TeamRespawnTime;
                long members = ParentClass->Teams[TeamID].MemberCount;
                if (IsDistanceBetweenThingsOver(Game->GetHero(), Me, 15.0f) && !Game->IsCameraPosOnScreen(Me.GetPos())) {
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    for (long i = 0; i < TeamMemberLimit - members; i++) {
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                        CScriptThing member = Game->CreateCreature(TeamMemberDefName, Me.GetPos(), TeamMemberName, false);
                        Game->EntityAttachToScript(member, "Q_OrchardFarmRaid");
                        Game->Pause(2.0f);
                    }
                } else {
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    for (long i = 0; i < TeamMemberLimit - members; i++) {
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                        CScriptThing member = Game->CreateCreature(TeamMemberDefName, OtherSpawnPoint.GetPos(), TeamMemberName, false);
                        Game->EntityAttachToScript(member, "Q_OrchardFarmRaid");
                        Game->Pause(2.0f);
                        OtherSpawnPoint = Game->GetRandomThingWithScriptName("EitherTeamSpawn");
                    }
                }
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
            }
        } else {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            if (TeamID == ParentClass->HeroTeam) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                Game->UpdateQuestInfoCounterList(BanditsLeftID, (2 - ParentClass->BanditWavesSpawned) * TeamMemberLimit + ParentClass->Teams[TeamID].MemberCount, -1);
            }
            if (ParentClass->Teams[TeamID].MemberCount == 0) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                Game->Pause((float)pGlobalScriptDef->BanditReinforcementDelay);
                ParentClass->BanditWavesSpawned++;
                if (TeamID != ParentClass->HeroTeam) {
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    CScriptThing guard = Game->GetNearestWithScriptName(Game->GetHero(), "GuardTeamMember");
                    if (guard.IsAlive() && IsDistanceBetweenThingsUnder(guard, Game->GetHero(), 15.0f)) {
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                        ParentClass->MakeTeamMemberComment("NEXT_WAVE", guard, (CQ_OrchardFarmRaidScript::ERaidComment)0);
                    }
                }
                if (ParentClass->BanditWavesSpawned == 3) {
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    if (TeamID == ParentClass->HeroTeam) {
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                        ParentClass->MissionFailed = 3;
                    }
                    return _Fail_Val.Return();
                }
                if (TeamID == ParentClass->HeroTeam) {
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                } else {
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
                long members = ParentClass->Teams[TeamID].MemberCount;
                for (long i = 0; i < TeamMemberLimit - members; i++) {
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    CScriptThing member;
                    if (i == 1 && ParentClass->HeroTeam == 0) {
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                        member = Game->CreateCreature("CREATURE_BANDIT_ARCHER_LEVEL1", Me.GetPos(), TeamMemberName, false);
                    } else {
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                        member = Game->CreateCreature(TeamMemberDefName, Me.GetPos(), TeamMemberName, false);
                    }
                    Game->EntityAttachToScript(member, "Q_OrchardFarmRaid");
                    Game->Pause(2.0f);
                }
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
            }
        }
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    Game->RemoveQuestInfoElement(BanditsLeftID);
}
