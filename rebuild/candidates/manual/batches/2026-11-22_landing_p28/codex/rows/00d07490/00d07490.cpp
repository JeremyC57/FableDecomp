// NScript::CQ_BanditCampBossBattleScript::CGate3Guard::Main() -- Gate3 guard greets the hero, hands over combat targeting after a hit, and handles the scripted conversations.
// Leaf: ?Main@CGate3Guard@CQ_BanditCampBossBattleScript@NScript@@EAEXXZ
// Flags: default
// Grade: DIFFER(2190v2182)

#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_NSCRIPT
#include "fable_compiler.h"
#include "engine/TRetVoid.h"
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
#define FABLE_RETAIL_PHASE1_ARCHER_PASS26
#include "engine/EScriptAIPriority.h"
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
inline CScriptThing::~CScriptThing() {}
inline NScript::CIsThingAlive::~CIsThingAlive() { }
extern const float g_fZero;                                  // 0x0122DEDC (0.0f)
extern CGameScriptInterfaceBase* pDSTGame;                   // 0x0143E8F8
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) { }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
inline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
inline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }

void NScript::CQ_BanditCampBossBattleScript::CGate3Guard::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid())
        seh_me.Release();
    while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    bool TalkedTo = false;
    bool SpokenToMe = false;
    CTimer MoanTimer;
    MoanTimer = 0;
    Game->SetQuestCardObjective("Q_BanditCamp", "TEXT_QUEST_BANDIT_CAMP_OBJECTIVE_06", "", "BanditCampMain");
    while (!IsActiveThreadTerminating()) {
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO")
            || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO")
                && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
            if (seh_me.IsValid())
                seh_me.Release();
            Game->ClearThingHasInformation(Me);
            while (true) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            // Inferred: the debug script generator retains this termination check after the idle loop.
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (!ParentClass.Gate3Open) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (MoanTimer.GetTime() <= 0 && IsDistanceBetweenThingsUnder(Game->GetHero(), Me, 8.0f)) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                long Conversation = Game->AddNewConversation(Me, false, false);
                Game->AddPersonToConversation(Conversation, Game->GetHero());
                if (!SpokenToMe) {
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    SpokenToMe = true;
                    Game->AddLineToConversation(Conversation, "TEXT_QST_009_BANDIT3_COMMENT_FIRST", false, Me, Game->GetHero());
                } else {
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    Game->AddLineToConversation(Conversation, "TEXT_QST_009_BANDIT3_COMMENT_SECOND", false, Me, Game->GetHero());
                }
                MoanTimer = 10;
            }
            if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                if (!TalkedTo) {
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    if (seh_me.IsValid())
                        seh_me.Release();
                    while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    do {
                        CScriptGameResourceObjectMovieBase speech_movie;
                        Game->StartMovieSequence("", speech_movie);
                        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                        if (Game->GetHealth(seh_me.GetScriptThing()) > g_fZero) {
                            seh_me.Speak(Game->GetHero(), "TEXT_QST_009_BANDIT3_COMMENT_FIRST_CHAT", GROUP_SELECT_NONE, false, true, false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating())
                                    return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating())
                                return _Fail_Val.Return();
                        }
                        TalkedTo = true;
                    } while (0);
                } else {
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    if (seh_me.IsValid())
                        seh_me.Release();
                    while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    do {
                        CScriptGameResourceObjectMovieBase speech_movie;
                        Game->StartMovieSequence("", speech_movie);
                        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                        if (Game->GetHealth(seh_me.GetScriptThing()) > g_fZero) {
                            seh_me.Speak(Game->GetHero(), "TEXT_QST_009_BANDIT3_COMMENT_SECOND_CHAT", GROUP_SELECT_NONE, false, true, false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating())
                                    return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating())
                                return _Fail_Val.Return();
                        }
                    } while (0);
                }
            }
        }
        Game->NewScriptFrame();
    }
}
