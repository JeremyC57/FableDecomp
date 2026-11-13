// NScript::CQ_WaspBossScript::CWaspChaseWoman::Main() -- Wasp Menace chased woman: wait for StartChase, run the ChasedWomanNav0..5 route while the WaspChaser lives, then thank the hero, give morality (first two villagers), run to VillagerEscapePos and fade out
// Leaf: ?Main@CWaspChaseWoman@CQ_WaspBossScript@NScript@@EAEXXZ
// Inline-budget: folded `&& true` on the route-update test -- it halves that arm's static frequency so its terminate `return`
//   calls ~CTimer 0x00CD4470 (retail 0x00E110BE `jne 0x00E1134E`) while the loop's own terminate `return` keeps the one expanded
//   DeregisterTimer pad 0x00E111B3 (c2 inline trace: ~CTimer c1 14, threshold f > 0.032; arm 0.044 -> 0.022, loop exit 0.117).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E10E60, 1306 bytes, thiscall, plain `ret`. Literals "WaspChaser" 0x012E01EC,
//   "ChasedWomanNav" 0x012E0464, "TEXT_QST_072_WASP_CHASE_FEMALE_ON_SAVED_10" 0x012E0438, "VillagerEscapePos" 0x012E0424.
// Identity (two sources): Ego_r.pdb / FableWin.pdb ?Main@CWaspChaseWoman@CQ_WaspBossScript@NScript@@EAEXXZ (private virtual), in
//   retail order right after the landed CWaspChaseWoman Alloc 0x00E10D70 / GetParentScript 0x00E10E10 / Init 0x00E10E20 /
//   scalar deleting dtor 0x00E10E40; the WaspBoss seed-corpus Lua port (refs/script_recovery/seed_corpus/sources/WaspBoss/
//   FSE/WaspBoss/Entities/WaspChaseWoman.lua) follows the same statements (QueenHornetAttacks removal, StartChase wait,
//   ChasedWomanNav2/3 start, (nav + 1) % 6 route while WaspChaser is alive, ON_SAVED_10 line, SavedVillagerCount < 2 morality,
//   VillagerEscapePos run, FadeOutAndKillEntity).
// Layout: CWaspChaseWoman (Ego_r 0x1c) ParentClass +0x14; CQ_WaspBossScript (engine header) QueenHornetAttacks +0x4b, StartChase
//   +0x4f, SavedVillagerCount +0x50; pGlobalScriptDef WB_MoralityGain +0xe30, WB_ScreamingVillagerFadeOutTime +0xe4c.
// Callees: as the landed sibling CWaspVictim::Main 0x00E11670, plus GFIntToCharString 0x0099F830, operator+(const char*, const
//   CCharString&) 0x0099F690, CScriptThing::operator= 0x008AB980, ScriptedThingBase FollowPreCalculatedRoute 0x007E7310 /
//   ClearCommands 0x007E7360; CTimer ctor 0x00CD4450 / dtor 0x00CD4470 (expanded at the chase loop's terminate exit).
// Shape (each measured against retail, chunk 2003):
//   - the chaser is a const reference bound to the returned temporary: IsAlive dispatches through the CScriptThing vtable
//     (slot 75, +0x12c) as retail 0x00E11192 does (a by-value local devirtualises it), as in the landed CWaspChaser::Main;
//   - the CTimer ctor is a same-TU noinline body (as the landed CWaspVictim::Main): VC7.1 then knows the timer does not escape,
//     so the "WaspChaser" / ChasedWomanNav3 / Nav2 string temporaries share its frame word [esp+10h] as retail does;
//   - the main loop is `for (;; NewScriptFrame()) { if (T) return; ... }` with the saved-by-hero code inside the not-alive arm
//     ending in `break`: retail's entry and bottom terminate tests both reach one expanded DeregisterTimer pad that jumps into
//     the shared tail 0x00E11357, and escape_pos (an arm-block local) shares [esp+3Ch] with the route temporaries.
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CQ_WASPBOSSSCRIPT_CHASEWOMAN
#define FABLE_RETAIL_SCRIPT_MOVE_ENUM
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
#include "engine/fable_script_text.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
extern CGameScriptInterfaceBase* pDSTGame;
FABLE_FORCEINLINE CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex,time); return *this; }
#ifdef _MSC_VER
__declspec(noinline)
#endif
CTimer::CTimer() { TimerIndex = pDSTGame->RegisterTimer(); }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
inline NScript::CIsThingAlive::~CIsThingAlive() { }
namespace NScript {
class CQ_WaspBossScript::CWaspChaseWoman : public CEntityScriptBase {
public:
    CQ_WaspBossScript*      ParentClass;                // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                // +0x18
    virtual CScriptBase* GetParentScript() const;
private:
    virtual void Init();
    virtual void Main();
};
}

void NScript::CQ_WaspBossScript::CWaspChaseWoman::Main()
{
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    if (ParentClass->QueenHornetAttacks) {
        if (IsActiveThreadTerminating()) return;
        Game->RemoveThing(Me, false, true);
    }
    while (!ParentClass->StartChase) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    const CScriptThing& chaser = Game->GetThingWithScriptName("WaspChaser");
    long nav = 2;
    CScriptThing next_nav = Game->GetThingWithScriptName("ChasedWomanNav" + GFIntToCharString(nav + 1));
    seh_me.FollowPreCalculatedRoute(Game->GetThingWithScriptName("ChasedWomanNav" + GFIntToCharString(nav)), ENTITY_MOVE_RUN, false, true);
    CTimer timer;
    timer = 11;
    for (;; Game->NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return;
        if ((!seh_me.IsPerformingScriptTask() || IsDistanceBetweenThingsUnder(Me, next_nav, 2.0f)) && true) {
            if (IsActiveThreadTerminating()) return;
            nav = (nav + 1) % 6;
            next_nav = Game->GetThingWithScriptName("ChasedWomanNav" + GFIntToCharString((nav + 1) % 6));
            seh_me.ClearCommands();
            seh_me.FollowPreCalculatedRoute(Game->GetThingWithScriptName("ChasedWomanNav" + GFIntToCharString(nav)), ENTITY_MOVE_RUN, false, true);
        }
        if (!chaser.IsAlive()) {
            if (IsActiveThreadTerminating()) return;
            Game->EntitySetAsScared(Me, false);
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            {
                CCharString line("TEXT_QST_072_WASP_CHASE_FEMALE_ON_SAVED_10");
                Game->AddLineToConversation(conversation, line, false, Me, Game->GetHero());
            }
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
                    seh_me.MoveToThing(escape_pos, 1.0f, ENTITY_MOVE_RUN, false, true, false, true);
                }
            }
            if (IsActiveThreadTerminating()) return;
            FadeOutAndKillEntity(Me, pGlobalScriptDef->WB_ScreamingVillagerFadeOutTime, true, true);
            break;
        }
    }
}
