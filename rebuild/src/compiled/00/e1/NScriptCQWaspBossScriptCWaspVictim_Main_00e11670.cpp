// NScript::CQ_WaspBossScript::CWaspVictim::Main() -- Wasp Menace villager: wait for QuestStartScreened, take Me under scripted control at 6 health, scream at the hero while the WaspAttacker lives, then thank the hero, give morality (first two villagers), run to VillagerEscapePos and fade out
// Leaf: ?Main@CWaspVictim@CQ_WaspBossScript@NScript@@EAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E11670, 1115 bytes, thiscall, plain `ret`. Literals "WaspAttacker" 0x012E01CC,
//   "TEXT_QST_072_VILLAGER_MALE_SCREAMS_10" 0x012E04D4, "TEXT_QST_072_VILLAGER_MALE_ON_SAVED_10" 0x012E04AC, "VillagerEscapePos"
//   0x012E0424; float 6.0f 0x0125A2A0.
// Identity (two sources): slot 1 (Main) of the entity vtable 0x012E0490 that the landed CWaspVictim::Alloc 0x00E11580 stores
//   (slot 2 = the landed Init 0x00E11630); Ego_r.pdb / FableWin.pdb ?Main@CWaspVictim@CQ_WaspBossScript@NScript@@EAEXXZ (private
//   virtual); the WaspBoss seed-corpus Lua port (refs/script_recovery/seed_corpus/sources/WaspBoss/.../WaspVictim.lua) follows
//   the same statements (scared villager, SCREAMS_10 / ON_SAVED_10 lines, SavedVillagerCount < 2 morality, escape run, fade).
// Layout: CWaspVictim (Ego_r 0x1c) ParentClass +0x14; CQ_WaspBossScript (engine header, retail offsets) QueenHornetAttacks +0x4b,
//   SavedVillagerCount +0x50, QuestStartScreened +0x54; pGlobalScriptDef (engine/CScriptDef.h) WB_MoralityGain +0xe30,
//   WB_ScreamingVillagerScreamsDistance +0xe48, WB_ScreamingVillagerFadeOutTime +0xe4c, WB_WaspChaseWomanPanicTime +0xe54.
// Callees: CIsThingAlive (vtable 0x012C32F0, ctor and dtor expanded, ~CScriptThing 0x004AA840 called), CScriptThing copy
//   0x004ABE90, SetPredicate 0x00F35B10, IsActiveThreadTerminating 0x00F35B30, CBaseObject() 0x0099A380, ScriptedThingBase
//   IsValid 0x00CD23B9 / Release 0x00CD2770 / IsPerformingScriptTask 0x007E7450 / MoveToThing 0x007E7300 / ~ 0x007E74D0, CTimer
//   0x00CD4450 / ~ 0x00CD4470 (GetTime and operator= expanded: pDSTGame 0x0143E8F8 slots 90 / 89), CCharString 0x0099EBF0 / ~
//   0x0099EAE0, IsDistanceBetweenThingsUnder 0x00CBE2FF / Over 0x00CBE3EA, FadeOutAndKillEntity 0x00CBE716 (__fastcall);
//   CGameScriptInterfaceBase slots 7, 8, 70, 72, 108, 156, 264, 265, 364, 365, 366, 496.
// Shape: shared headers (CEntityScriptBase / CIsThingAlive / NScript_CQ_WaspBossScript with the new FABLE_RETAIL_CQ_WASPBOSSSCRIPT_ENTITIES
//   nested declaration). The CTimer ctor and dtor are same-TU `__declspec(noinline)` bodies (GOTCHAS: VC7.1 then knows the timer does not
//   escape and keeps TimerIndex in ebx across the scream loop, and the 0 of the object ctor in ebx); with header-only declarations
//   ebx/ebp swap (dl=66..81). operator= is `__forceinline` (as 24 landed CTimer rows): plain inline stays a call at this site.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CQ_WASPBOSSSCRIPT_ENTITIES
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CIsThingAlive.h"
#include "engine/CEntityScriptBase.h"
#include "engine/NScript_CQ_WaspBossScript.h"
#include "engine/CTimer.h"
#include "engine/fable_script_def_global.h"
#include "engine/fable_script_distance.h"
#include "engine/fable_script_fade.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum EScriptEntityMoveType { };
extern CGameScriptInterfaceBase* pDSTGame;
__declspec(noinline) CTimer::CTimer() { TimerIndex = pDSTGame->RegisterTimer(); }
__declspec(noinline) CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex,time); return *this; }
inline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
inline bool CScriptThing::IsAlive() const { return !PImp.IsNull() && PImp->IsAlive(); }
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
inline NScript::CIsThingAlive::~CIsThingAlive() { }
namespace NScript {
class CQ_WaspBossScript::CWaspVictim : public CEntityScriptBase {
public:
    CQ_WaspBossScript*      ParentClass;                // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                // +0x18
    virtual CScriptBase* GetParentScript() const;
private:
    virtual void Init();
    virtual void Main();
};
}

void NScript::CQ_WaspBossScript::CWaspVictim::Main()
{
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    while (!ParentClass->QuestStartScreened) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    Game->ModifyThingHealth(Me, 6.0f - Game->GetHealth(Me), false);
    if (ParentClass->QueenHornetAttacks) {
        if (IsActiveThreadTerminating()) return;
        Game->RemoveThing(Me, false, true);
    }
    CScriptThing attacker = Game->GetThingWithScriptName("WaspAttacker");
    CTimer scream_timer;
    while (attacker.IsAlive()) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (scream_timer.GetTime() == 0) {
            if (IsActiveThreadTerminating()) return;
            if (IsDistanceBetweenThingsUnder(Me, Game->GetHero(), (float)pGlobalScriptDef->WB_ScreamingVillagerScreamsDistance)) {
                if (IsActiveThreadTerminating()) return;
                scream_timer = pGlobalScriptDef->WB_WaspChaseWomanPanicTime;
                long conversation = Game->AddNewConversation(Me, false, false);
                Game->AddPersonToConversation(conversation, Game->GetHero());
                {
                    CCharString line("TEXT_QST_072_VILLAGER_MALE_SCREAMS_10");
                    Game->AddLineToConversation(conversation, line, false, Me, Game->GetHero());
                }
            }
        }
    }
    if (IsActiveThreadTerminating()) return;
    long conversation = Game->AddNewConversation(Me, false, false);
    Game->AddPersonToConversation(conversation, Game->GetHero());
    {
        CCharString line("TEXT_QST_072_VILLAGER_MALE_ON_SAVED_10");
        Game->AddLineToConversation(conversation, line, false, Me, Game->GetHero());
    }
    Game->EntitySetAsScared(Me, false);
    CScriptThing escape_pos = Game->GetThingWithScriptName("VillagerEscapePos");
    if (ParentClass->SavedVillagerCount < 2) {
        if (IsActiveThreadTerminating()) return;
        Game->GiveHeroMorality(pGlobalScriptDef->WB_MoralityGain);
        ParentClass->SavedVillagerCount++;
    }
    while (IsDistanceBetweenThingsOver(Me, escape_pos, 2.0f)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (!seh_me.IsPerformingScriptTask()) {
            if (IsActiveThreadTerminating()) return;
            seh_me.MoveToThing(escape_pos, 1.0f, (EScriptEntityMoveType)1, false, true, false, true);
        }
    }
    if (IsActiveThreadTerminating()) return;
    FadeOutAndKillEntity(Me, pGlobalScriptDef->WB_ScreamingVillagerFadeOutTime, true, true);
}