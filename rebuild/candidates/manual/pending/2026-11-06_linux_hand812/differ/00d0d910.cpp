// NScript::CQ_BanditCampBossBattleScript::CGate2Guard1::Main() -- the second bandit-camp gate guard: script himself; while the gate is shut, hit lines when the hero strikes him (the fourth makes him a killable enemy), a passing comment when the hero is close, and when talked to either the gate-blocked line or (with the camp pass) the given-pass line, the gate opened by the parent's OpenGate thread and the pass conversation; once the gate is open, an aside when talked to and, when attacked, the attacked line before he turns on the hero
// Grade: DIFFER(4928v4752) -- SDIFF 304 (551 lines): the movie dtor and ~CTimer inline budget: retail expands the implicit movie dtor at its 9 sites (5 scope ends + 4 shared exit pads) and ~CTimer at 2 of 3 sites; plain inline (a1, 1305 lines / SDIFF 362) calls the movie COMDAT at 12 of 15 sites (ours keeps more exit copies), MOVIEBASE_FORCEINLINE adds the MovieBase vptr store, and the __forceinline ~CTimer of this source expands every exit (+176 B); the loop-head thread exit expands the epilogue in place where retail jumps to a shared pad; eax/edx colouring in the switch arms
// Next idea: the movie-dtor budget (chunk 810 00d1b200) first: with the implicit dtor expanding the exit pads should share as in retail, then re-try plain inline ~CTimer
// Leaf: ?Main@CGate2Guard1@CQ_BanditCampBossBattleScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D0D910, 4752 bytes (incl. the switch table 0x00D0EB90), thiscall, plain `ret`, no EH frame.
// Identity (proven, two sources): Ego_r.pdb and FableWin.pdb name sets hold ?Main@CGate2Guard1@CQ_BanditCampBossBattleScript@NScript@@EAEXXZ;
//   one .rdata reference, slot 1 of the entity vtable 0x012C8E80 stored by the landed CGate2Guard1::Alloc 0x00D071F0.
// Callees: OpenGate 0x00D0EBC0 through CSpawnedFunc2<CQ_BanditCampBossBattleScript,float,CCharString> (vtable 0x012C9918, SuspendableProcess 0x00D0EBA0).
// Layout: engine/CQ_BanditCampBossBattleScript.h with the chunk 812 GATE2GUARD1 opt-in (hdr/README.md).
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_FORCEINLINE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CQ_BANDITCAMPBOSSBATTLESCRIPT_GATE2GUARD1
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_BanditCampBossBattleScript.h"
#include "engine/CIsThingAlive.h"
#include "engine/CRGBColour.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
class CScriptGameResourceObjectMovieBase;
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CTimer.h"
#include "engine/CScriptDef.h"
#include "engine/CDefPointer.h"
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& t):Thing(t) {}
inline NScript::CIsThingAlive::~CIsThingAlive() { }
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing&, const CScriptThing&, float);   // 0x00CBE2FF
extern CDefPointer<const CScriptDef> pGlobalScriptDef;                  // 0x0143E90C
extern CGameScriptInterfaceBase* pDSTGame;                              // 0x0143E8F8
inline CTimer::CTimer():TimerIndex(pDSTGame->RegisterTimer()) {}
__forceinline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex,time); return *this; }
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
template <class T, class P1, class P2> class CSpawnedFunc2 : public CSpawnedFuncBase {   // CSpawnedFunc2<CQ_BanditCampBossBattleScript, float, CCharString>: vtable 0x012C9918
public:
    __forceinline CSpawnedFunc2(const CCharString& name, unsigned long id, T* pclass, void (T::*func)(P1, P2), P1 par1, P2 par2)
        : CSpawnedFuncBase(name, id), Func(func), PClass(pclass), Par1(par1), Par2(par2) { }
    virtual void SuspendableProcess() { (PClass->*Func)(Par1, Par2); StopRunning = true; }   // 0x00D0EBA0
    void (T::*Func)(P1, P2);                                             // +0x34
    T* PClass;                                                           // +0x38
    P1 Par1;                                                             // +0x3c
    P2 Par2;                                                             // +0x40
};

void NScript::CQ_BanditCampBossBattleScript::CGate2Guard1::Main()
{
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid())
        seh_me.Release();
    while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->EntitySetAsKillable(Me, false, true);
    CTimer CommentTimer;
    CommentTimer = 0;
    bool Commented = false;
    long HitCount = 0;
    while (!IsActiveThreadTerminating()) {
        if (!ParentClass.Gate2Open) {
            if (IsActiveThreadTerminating())
                return;
            if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
                if (IsActiveThreadTerminating())
                    return;
                CScriptGameResourceObjectMovieBase movie;
                Game->StartMovieSequence("", movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                switch (HitCount) {
                case 0:
                    if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game->GetHero(), "TEXT_QST_009_BANDIT2_HIT_FIRST", GROUP_SELECT_NONE, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating())
                                return;
                        }
                        if (IsActiveThreadTerminating())
                            return;
                    }
                    Me.SetFriendsWithEverythingFlag(true);
                    HitCount = 1;
                    break;
                case 1:
                    if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game->GetHero(), "TEXT_QST_009_BANDIT2_HIT_SECOND", GROUP_SELECT_NONE, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating())
                                return;
                        }
                        if (IsActiveThreadTerminating())
                            return;
                    }
                    Me.SetFriendsWithEverythingFlag(true);
                    break;
                case 2:
                    if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game->GetHero(), "TEXT_QST_009_BANDIT2_HIT_THIRD", GROUP_SELECT_NONE, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating())
                                return;
                        }
                        if (IsActiveThreadTerminating())
                            return;
                    }
                    Me.SetFriendsWithEverythingFlag(true);
                    break;
                case 3:
                    if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game->GetHero(), "TEXT_QST_009_BANDIT2_HIT_FOURTH", GROUP_SELECT_NONE, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating())
                                return;
                        }
                        if (IsActiveThreadTerminating())
                            return;
                    }
                    Me.SetFriendsWithEverythingFlag(false);
                    Game->ModifyThingHealth(Me, 10000.0f, false);
                    Game->EntitySetAsKillable(Me, true, true);
                    Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
                    Game->ClearThingHasInformation(Me);
                    break;
                }
            }
            if (CommentTimer.GetTime() <= 0 && IsDistanceBetweenThingsUnder(Game->GetHero(), Me, 8.0f)) {
                if (IsActiveThreadTerminating())
                    return;
                long HLStyleConversationIndex = Game->AddNewConversation(Me, false, false);
                Game->AddPersonToConversation(HLStyleConversationIndex, Game->GetHero());
                if (!Commented) {
                    if (IsActiveThreadTerminating())
                        return;
                    Commented = true;
                    Game->AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_009_BANDIT2_COMMENT_FIRST", false, Me, Game->GetHero());
                } else {
                    if (IsActiveThreadTerminating())
                        return;
                    Game->AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_009_BANDIT2_COMMENT_SECOND", false, Me, Game->GetHero());
                }
                CommentTimer = 10;
            }
            if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                if (IsActiveThreadTerminating())
                    return;
                ParentClass.SpokenToSecondGuard = true;
                if (seh_me.IsValid())
                    seh_me.Release();
                while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
                if (IsActiveThreadTerminating())
                    return;
                if (!Game->IsObjectInThingsPossession("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS", Game->GetHero())) {
                    if (IsActiveThreadTerminating())
                        return;
                    CScriptGameResourceObjectMovieBase movie;
                    Game->StartMovieSequence("", movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game->GetHero(), "TEXT_QST_009_BANDIT2_GATE_BLOCKED", GROUP_SELECT_NONE, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating())
                                return;
                        }
                        if (IsActiveThreadTerminating())
                            return;
                    }
                } else {
                    if (IsActiveThreadTerminating())
                        return;
                    Game->GiveHeroExperience(pGlobalScriptDef->EXPERIENCE_GRANT_MEDIUM);
                    {
                        CScriptGameResourceObjectMovieBase movie;
                        Game->StartMovieSequence("", movie);
                        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                        if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                            seh_me.Speak(Game->GetHero(), "TEXT_QST_009_BANDIT2_GIVEN_CAMP_PASS", GROUP_SELECT_NONE, false, true, false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating())
                                    return;
                            }
                            if (IsActiveThreadTerminating())
                                return;
                        }
                    }
                    ParentClass.Gate2Open = true;
                    ParentClass.AddSpawnedFunction(new CSpawnedFunc2<CQ_BanditCampBossBattleScript, float, CCharString>(
                        CCharString("ParentClass.", -1) + CCharString("OpenGate", -1), 0, &ParentClass,
                        &CQ_BanditCampBossBattleScript::OpenGate, 2.0f, "Gate2Outer"), CCharString(""));
                    long HLStyleConversationIndex = Game->AddNewConversation(Me, false, false);
                    Game->AddPersonToConversation(HLStyleConversationIndex, Game->GetHero());
                    Game->AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_009_BANDIT2_PASS", false, Me, Game->GetHero());
                    Game->ClearThingHasInformation(Me);
                }
                if (seh_me.IsValid())
                    seh_me.Release();
                while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
                if (IsActiveThreadTerminating())
                    return;
            }
        } else {
            if (IsActiveThreadTerminating())
                return;
            if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                if (IsActiveThreadTerminating())
                    return;
                CScriptGameResourceObjectMovieBase movie;
                Game->StartMovieSequence("", movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game->GetHero(), "TEXT_QST_009_BANDIT2_ASIDE", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return;
                    }
                    if (IsActiveThreadTerminating())
                        return;
                }
            }
            if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
                if (IsActiveThreadTerminating())
                    return;
                {
                    CScriptGameResourceObjectMovieBase movie;
                    Game->StartMovieSequence("", movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game->GetHero(), "TEXT_QST_009_BANDIT2_ATTACKED_NEW", GROUP_SELECT_NONE, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating())
                                return;
                        }
                        if (IsActiveThreadTerminating())
                            return;
                    }
                }
                Me.SetFriendsWithEverythingFlag(false);
                Game->EntitySetAsKillable(Me, true, true);
                Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
                Game->ClearThingHasInformation(Me);
                if (seh_me.IsValid())
                    seh_me.Release();
                for (;;) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
            }
        }
        Game->NewScriptFrame();
    }
}
