// NScript::CQ_BanditCampBossBattleScript::CGate3Guard::Main() -- the bandit camp's third-gate guard: hold an alive filter over Me, take Me under scripted control (priority 4), set the BanditCampMain quest-card objective, then each frame: when the hero hits him (plain hit, or a special ability other than 14) make the hero his enemy, clear his information and idle for good; while Gate3 is shut, every 10 s within 8 units comment at the hero (FIRST, then SECOND) and, when talked to, re-take control and play a paused-movie chat line (FIRST_CHAT once, then SECOND_CHAT)
// Grade: DIFFER(2089v2182) -- VC7.1 inline split of the timer / movie dtors per exit site: retail expands ~CTimer at the loop exit, the hit-path returns and the idle loop (3 tails with their own epilogue) and calls 0x00CD4470 at the deeper returns, while expanding the movie dtor (0x006E7AB0 + vptr 0x0126008C + ~CBaseObject) at all four movie sites; the timer index then lives in ebp. No spelling found that heats the hit path without heating the movie exits.
// Next idea: measure per-site frequency with marker calls (INLINE_DEPTH_REVISIT.md lever 4/5); try the hit test as a separate bool local and the Gate3 block as an else-arm; the frame / call sequence already matches retail.
// Leaf: ?Main@CGate3Guard@CQ_BanditCampBossBattleScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D07490, 2182 bytes, thiscall, plain ret; entity-script vtable 0x012C8E9C slot 1 (Main).
// Identity (proven): Ego_r.pdb ?Main@CGate3Guard@CQ_BanditCampBossBattleScript@NScript@@EAEXXZ (0x00D09D10 in Ego_r, right after its Alloc
//   / Init, the retail donor order 0x00D07340 Alloc, 0x00D07410 Init, 0x00D07490 Main); the landed Alloc 0x00D07340 stores the vtable
//   whose slot 1 is this; the literals are the TU's (TEXT_QST_009_BANDIT3_*, Q_BanditCamp, BanditCampMain) and Gate3Open +0x4d is the
//   CQ_BanditCampBossBattleScript member (Ego_r +0x61 - 0x14). The ledger's under-qualified NScript::CGate3Guard::Main is this.
// Callees: CScriptThing copy 0x004ABE90 / dtor 0x004AA840, CEntityScriptBase::SetPredicate 0x00F35B10 / IsActiveThreadTerminating
//   0x00F35B30, CBaseObject() 0x0099A380 / ~ 0x0099A430, scripted-thing handle IsValid 0x00CD23B9 / Release 0x00CD2770 /
//   GetScriptThing 0x007E7490 / Speak 0x007E7390 / IsPerformingScriptTask 0x007E7450 / dtor 0x007E74D0, ~CCountedPointer<
//   CScriptGameResourceObjectMovieBase> 0x006E7AB0, ~CTimer 0x00CD4470, IsDistanceBetweenThingsUnder 0x00CBE2FF, CCharString 0x0099EBF0
//   / 0x0099EAE0; CGameScriptInterfaceBase slots 7, 8, 70 GetHero, 264 GetHealth, 296 SetQuestCardObjective, 361
//   ClearThingHasInformation, 364/365/366 conversation, 370 StartMovieSequence, 379 PauseAllNonScriptedEntities, 451
//   GiveThingBestEnemyTarget; pDSTGame 87 RegisterTimer / 88 DeregisterTimer / 89 SetTimer / 90 GetTimer; CScriptThing slots 21, 27,
//   41, 42; g_fZero 0x0122DEDC.
// Model: shared headers (new hdr/engine/CQ_BanditCampBossBattleScript.h); the CDF_Dragon movie / pause model; do-while(0)
//   statement-macro terminate checks inside the speak loops and the movie block as a do-while(0) scope were the closest (attempt j).
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_NSCRIPT
#include "engine/CQ_BanditCampBossBattleScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CIsThingAlive.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/ETextGroupSelectionMethod.h"
#include "engine/EHeroAbility.h"
#include "engine/CTimer.h"
#include "engine/fable_script_distance.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST = 0, SCRIPT_AI_PRIORITY_LOW = 1, SCRIPT_AI_PRIORITY_MEDIUM = 2,
                         SCRIPT_AI_PRIORITY_HIGH = 3, SCRIPT_AI_PRIORITY_HIGHEST = 4 };
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
inline NScript::CIsThingAlive::~CIsThingAlive() { }
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();   // 0x006E7AB0 (out of line in retail)
extern const float g_fZero;                                  // 0x0122DEDC (0.0f)
extern CGameScriptInterfaceBase* pDSTGame;                   // 0x0143E8F8
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) { }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }

void NScript::CQ_BanditCampBossBattleScript::CGate3Guard::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase guard;
    if (guard.IsValid())
        guard.Release();
    while (!Game->StartScriptingEntity(Me, guard, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    bool chatted = false;
    bool commented = false;
    CTimer commentTimer;
    commentTimer = 0;
    Game->SetQuestCardObjective("Q_BanditCamp", "TEXT_QUEST_BANDIT_CAMP_OBJECTIVE_06", "", "BanditCampMain");
    while (!IsActiveThreadTerminating()) {
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO")
            || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO")
                && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            do { if (IsActiveThreadTerminating()) return; } while (0);
            Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
            if (guard.IsValid())
                guard.Release();
            Game->ClearThingHasInformation(Me);
            for (;;) {
                Game->NewScriptFrame();
                do { if (IsActiveThreadTerminating()) return; } while (0);
            }
        }
        if (!ParentClass.Gate3Open) {
            do { if (IsActiveThreadTerminating()) return; } while (0);
            if (commentTimer.GetTime() <= 0 && IsDistanceBetweenThingsUnder(Game->GetHero(), Me, 8.0f)) {
                if (IsActiveThreadTerminating())
                    return;
                long conversation = Game->AddNewConversation(Me, false, false);
                Game->AddPersonToConversation(conversation, Game->GetHero());
                if (!commented) {
                    if (IsActiveThreadTerminating())
                        return;
                    commented = true;
                    Game->AddLineToConversation(conversation, "TEXT_QST_009_BANDIT3_COMMENT_FIRST", false, Me, Game->GetHero());
                } else {
                    if (IsActiveThreadTerminating())
                        return;
                    Game->AddLineToConversation(conversation, "TEXT_QST_009_BANDIT3_COMMENT_SECOND", false, Me, Game->GetHero());
                }
                commentTimer = 10;
            }
            if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                if (IsActiveThreadTerminating())
                    return;
                if (!chatted) {
                    if (IsActiveThreadTerminating())
                        return;
                    if (guard.IsValid())
                        guard.Release();
                    while (!Game->StartScriptingEntity(Me, guard, SCRIPT_AI_PRIORITY_HIGHEST)) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return;
                    }
                    if (IsActiveThreadTerminating())
                        return;
                    do {
                        CScriptGameResourceObjectMovieBase movie;
                        Game->StartMovieSequence("", movie);
                        CWideScreenMagicPauseEntities pause(*Game);
                        if (Game->GetHealth(guard.GetScriptThing()) > g_fZero) {
                            guard.Speak(Game->GetHero(), "TEXT_QST_009_BANDIT3_COMMENT_FIRST_CHAT", GROUP_SELECT_NONE, false, true, false);
                            while (guard.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating())
                                    return;
                            }
                            if (IsActiveThreadTerminating())
                                return;
                        }
                        chatted = true;
                    } while (0);
                } else {
                    if (IsActiveThreadTerminating())
                        return;
                    if (guard.IsValid())
                        guard.Release();
                    while (!Game->StartScriptingEntity(Me, guard, SCRIPT_AI_PRIORITY_HIGHEST)) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return;
                    }
                    if (IsActiveThreadTerminating())
                        return;
                    do {
                        CScriptGameResourceObjectMovieBase movie;
                        Game->StartMovieSequence("", movie);
                        CWideScreenMagicPauseEntities pause(*Game);
                        if (Game->GetHealth(guard.GetScriptThing()) > g_fZero) {
                            guard.Speak(Game->GetHero(), "TEXT_QST_009_BANDIT3_COMMENT_SECOND_CHAT", GROUP_SELECT_NONE, false, true, false);
                            while (guard.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating())
                                    return;
                            }
                            if (IsActiveThreadTerminating())
                                return;
                        }
                    } while (0);
                }
            }
        }
        Game->NewScriptFrame();
    }
}
