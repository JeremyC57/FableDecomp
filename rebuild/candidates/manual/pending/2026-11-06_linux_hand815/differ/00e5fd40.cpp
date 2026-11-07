// NScript::CV_BodyGuardScript::CBodyGuard::Main() -- a hireable henchman: keep the alive predicate, teleport to the hero when already hired; while for hire turn hostile for 20 s when hit, ask for attention when targeted and run the hire conversation (too expensive / too many / hire question); while hired react to the hold order, the hero's hits (anger warnings, quitting), his own health, the hero's weapon (freak out / unsheathe / sheathe), the fire question, heals, summoner hits and state changes, and quit when the hero can no longer pay
// Grade: DIFFER(9535v9381) -- 887 md lines: retail expands ~CScriptThing for the first 2 of 8 temporaries (GetHeroTargetedThing, TOO_EXPENSIVE GetScriptThing) and calls 0x004AA840 for the rest; row-local inline dtor expands 4 (a1, 891 lines), none without (a4); exit-path tails cross-jumped differently; temp slot order
// Next idea: find what consumes VC7.1's inline budget earlier in retail (GOTCHAS 2026-11-06 chunk 803: open); then the duplicated return tails
// Leaf: ?Main@CBodyGuard@CV_BodyGuardScript@NScript@@EAEXXZ
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CV_BODYGUARDSCRIPT_CBODYGUARD_MAIN
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "stlport/fable_containers.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CV_BodyGuardScript.h"
#include "engine/CIsThingAlive.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
class CScriptGameResourceObjectMovieBase;
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CTimer.h"
#include "engine/CScriptDef.h"
#include "engine/CDefPointer.h"
#include "engine/EHeroAbility.h"
#include "engine/fable_script_distance.h"
#include "engine/TRetVoid.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A
extern CDefPointer<const CScriptDef> pGlobalScriptDef;   // 0x0143E90C
float FABLE_FASTCALL GetDistanceBetweenThings(const CScriptThing&, const CScriptThing&);   // 0x00CBE512
__forceinline bool CScriptThing::operator==(const CScriptThing& other) const { return PImp->IsEqualTo(*other.PImp); }
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) { }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
#define BG_T if (IsActiveThreadTerminating()) return _Fail_Val.Return()
#define BG_START_SCRIPTING(thing, seh) \
    if (seh.IsValid()) seh.Release(); \
    while (!Game->StartScriptingEntity(thing, seh, SCRIPT_AI_PRIORITY_HIGHEST)) { \
        Game->NewScriptFrame(); \
        BG_T; \
    } \
    BG_T
// TEXT: the henchman's own text key, "TEXT_QST_016_BODYGUARD" + <data string> + "_" + <suffix>.
#define BG_TEXT(suffix) ("TEXT_QST_016_BODYGUARD" + Me.GetDataString() + "_" + suffix)
#define BG_SPEAK(suffix) \
    if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) { \
        seh_me.Speak(Game->GetHero(), BG_TEXT(suffix), GROUP_SELECT_NONE, false, true, false); \
        while (seh_me.IsPerformingScriptTask()) { \
            Game->NewScriptFrame(); \
            BG_T; \
        } \
        BG_T; \
    }
#define BG_SAY(suffix) { \
        long conversation = Game->AddNewConversation(Me, false, false); \
        Game->AddPersonToConversation(conversation, Game->GetHero()); \
        Game->AddLineToConversation(conversation, BG_TEXT(suffix), false, Me, Game->GetHero()); \
    }
// HIT: hit by the attacker, or by one of its special abilities that is not the hero's heal spell.
#define BG_HIT(attacker) (Me.MsgIsHitBy(attacker) || (Me.MsgIsHitByAnySpecialAbilityFrom(attacker) && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO")))
#define BG_COMMENT(text) ParentClass.MakeHenchmanComment(text, 1, CV_BodyGuardScript::HM_COMMENT_SINGLE, Me)

void NScript::CV_BodyGuardScript::CBodyGuard::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    BG_T;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    HenchManType = ParentClass.GetHenchmanTypeFromString(Me.GetDataString());
    if (BrainState == 2) {
        BG_T;
        SetHenchmanAsHired(true);
        Game->EntityTeleportToHeroPosition(Me);
    }
    CCharString unusedText;
    long currentStateGroup = 0;
    long lastStateGroup = 0;
    CTimer FreakedOutTimer;
    CTimer AttentionTimer;
    for (; !IsActiveThreadTerminating(); Game->NewScriptFrame()) {
        while (BrainState == 1) {
            Game->NewScriptFrame();
            BG_T;
            if (BG_HIT("SCRIPT_NAME_HERO")) {
                BG_T;
                Game->EntityUnsetThingAsAllyOfThing(Me, Game->GetHero());
                Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
                Game->ClearThingHasInformation(Me);
                Game->MiniMapAddMarker(Me, "HUD_ORB_RED_SMALL");
                CTimer HostileTimer;
                HostileTimer = 20;
                while (HostileTimer.GetTime() > 0) {
                    Game->NewScriptFrame();
                    BG_T;
                    if (BG_HIT("SCRIPT_NAME_HERO")) {
                        BG_T;
                        HostileTimer = 20;
                    }
                }
                BG_T;
                Game->MiniMapRemoveMarker(Me);
                Game->EntitySetThingAsAllyOfThing(Me, Game->GetHero());
                Game->ClearThingBestEnemyTarget(Me);
                Game->SetThingHasInformation(Me, false, true, false);
            }
            if (AttentionTimer.GetTime() == 0 && Game->GetHeroTargetedThing() == Me) {
                BG_T;
                AttentionTimer = 10;
                BG_SAY("GET_EMPLOYER_ATTENTION")
            }
            if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                BG_T;
                BG_START_SCRIPTING(Me, seh_me);
                CScriptGameResourceObjectMovieBase hire_movie;
                Game->StartMovieSequence("", hire_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (Game->GetHeroGold() < 30) {
                    BG_T;
                    BG_SPEAK("TOO_EXPENSIVE")
                    Game->DisplayGameInfo("TEXT_QST_016_NEED_THIS_MUCH");
                    while (!Game->MsgIsGameInfoClickedPast()) {
                        Game->NewScriptFrame();
                        BG_T;
                    }
                    BG_T;
                } else if (ParentClass.HenchmanList.size() >= 2) {
                    BG_T;
                    BG_SPEAK("TOO_MANY")
                } else {
                    BG_T;
                    BG_SPEAK("INTRO")
                    Game->GiveHeroYesNoQuestion(BG_TEXT("QUESTION_HIRE"), BG_TEXT("QUESTION_YES"), BG_TEXT("QUESTION_NO"), "", true);
                    long answer = Game->MsgIsQuestionAnsweredYesOrNo();
                    while (answer < 0) {
                        Game->NewScriptFrame();
                        BG_T;
                        answer = Game->MsgIsQuestionAnsweredYesOrNo();
                    }
                    BG_T;
                    if (answer == 1) {
                        BG_T;
                        BG_SPEAK("HIRED")
                        SetHenchmanAsHired(true);
                    } else {
                        BG_T;
                        BG_SPEAK("NOT_HIRED")
                    }
                }
                if (seh_me.IsValid()) seh_me.Release();
            }
        }
        while (BrainState == 2) {
            Game->NewScriptFrame();
            BG_T;
            if (PMasterData->HenchmanOverrideState == 2) {
                BG_T;
                SetHenchmanAsHired(false);
                while (true) {
                    Game->NewScriptFrame();
                    BG_T;
                }
            }
            if (PMasterData->BodyGuardsMustStandAndWait) {
                BG_T;
                CScriptGameResourceObjectScriptedThingBase seh_wait;
                BG_START_SCRIPTING(Me, seh_wait);
                while (PMasterData->BodyGuardsMustStandAndWait) {
                    Game->NewScriptFrame();
                    BG_T;
                }
                BG_T;
                if (seh_wait.IsValid()) seh_wait.Release();
            }
            if (BG_HIT("SCRIPT_NAME_HERO")) {
                BG_T;
                if (AngerTimer.GetTime() < 20) {
                    BG_T;
                    if (BG_COMMENT("WARNING_LOW")) {
                        BG_T;
                        AngerTimer = AngerTimer.GetTime() + 30;
                    }
                } else if (AngerTimer.GetTime() < 45) {
                    BG_T;
                    if (BG_COMMENT("WARNING_MEDIUM")) {
                        BG_T;
                        AngerTimer = AngerTimer.GetTime() + 30;
                    }
                } else if (AngerTimer.GetTime() < 70) {
                    BG_T;
                    if (BG_COMMENT("WARNING_HIGH")) {
                        BG_T;
                        SetHenchmanAsHired(false);
                        BG_T;
                        AngerTimer = AngerTimer.GetTime() + 30;
                    }
                }
            }
            long health = (long)Game->GetHealth(Me);
            if (health < pGlobalScriptDef->HenchmanHealthCritical) {
                BG_T;
                CurrentHealthState = 2;
            } else if (health < pGlobalScriptDef->HenchmanHealthLow) {
                BG_T;
                CurrentHealthState = 1;
            } else {
                BG_T;
                CurrentHealthState = 0;
            }
            if (CurrentHealthState != PreviousHealthState) {
                BG_T;
                CCharString comment("");
                if (CurrentHealthState < PreviousHealthState) {
                    BG_T;
                    comment = "HEALED";
                } else {
                    BG_T;
                    switch (CurrentHealthState) {
                    case 0: comment = "HEALTH_FINE"; break;
                    case 1: comment = "HEALTH_LOW"; break;
                    case 2: comment = "HEALTH_VERY_LOW"; break;
                    }
                }
                if (BG_COMMENT(comment)) {
                    BG_T;
                    PreviousHealthState = CurrentHealthState;
                }
            }
            if (FreakedOut && FreakedOutTimer.GetTime() == 0) {
                BG_T;
                FreakedOut = false;
                if (currentStateGroup != 1) {
                    BG_T;
                    Game->EntitySheatheWeapons(Me, true);
                    BG_COMMENT("NOT_FREAKED");
                }
            }
            if (Me.GetCurrentStateGroupType() == 2 && IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 10.0f)) {
                BG_T;
                if (ParentClass.HenchmanFreakedOutThePlayerTimer.GetTime() == 0 && !ParentClass.HeroWieldingWeapon && !Game->IsEntityWieldingWeapon(Me)) {
                    BG_T;
                    Game->EntityUnsheatheWeapons(Me, true);
                    BG_COMMENT("FREAKED");
                    ParentClass.HenchmanFreakedOutThePlayerTimer = 180;
                    FreakedOutTimer = 6;
                    FreakedOut = true;
                } else if (!FreakedOut && ParentClass.HeroWieldingWeapon && !Game->IsEntityWieldingWeapon(Me)) {
                    BG_T;
                    float pauseTime = GetDistanceBetweenThings(Me, Game->GetHero()) * 0.5f;
                    if (pauseTime > 3.0f) pauseTime = 3.0f;
                    Game->Pause(pauseTime);
                    Game->EntityUnsheatheWeapons(Me, true);
                    BG_COMMENT("UNSHEATHE");
                } else if (!FreakedOut && !ParentClass.HeroWieldingWeapon && Game->IsEntityWieldingWeapon(Me)) {
                    BG_T;
                    float pauseTime = GetDistanceBetweenThings(Me, Game->GetHero()) * 0.5f;
                    if (pauseTime > 3.0f) pauseTime = 3.0f;
                    Game->Pause(pauseTime);
                    Game->EntitySheatheWeapons(Me, true);
                }
            }
            if (!ParentClass.HeroCannotAffordHelp) {
                if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                    BG_T;
                    BG_START_SCRIPTING(Me, seh_me);
                    CScriptGameResourceObjectMovieBase fire_movie;
                    Game->StartMovieSequence("", fire_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    Game->GiveHeroYesNoQuestion(BG_TEXT("QUESTION_FIRE"), BG_TEXT("QUESTION_YES"), BG_TEXT("QUESTION_NO"), "", true);
                    long answer = Game->MsgIsQuestionAnsweredYesOrNo();
                    while (answer < 0) {
                        Game->NewScriptFrame();
                        BG_T;
                        answer = Game->MsgIsQuestionAnsweredYesOrNo();
                    }
                    BG_T;
                    if (answer == 1) {
                        BG_T;
                        BG_SPEAK("FIRED")
                        BrainState = 3;
                    } else {
                        BG_T;
                        BG_SPEAK("NOT_FIRED")
                    }
                    if (seh_me.IsValid()) seh_me.Release();
                }
                if (Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO")) {
                    BG_T;
                    BG_COMMENT("HEALED");
                }
                if (AttackedBySummonersTimer.GetTime() == 0 && BG_HIT("SummonerAttacker")) {
                    BG_T;
                    if (BG_COMMENT("SUMMONERS")) {
                        BG_T;
                        AttackedBySummonersTimer = 30;
                    }
                }
                currentStateGroup = Me.GetCurrentStateGroupType();
                if (lastStateGroup != currentStateGroup) {
                    BG_T;
                    if (currentStateGroup == 2) {
                        BG_T;
                        BG_COMMENT("FOLLOWING");
                    } else if (currentStateGroup == 1) {
                        BG_T;
                        BG_COMMENT("ATTACKING");
                    }
                }
                lastStateGroup = currentStateGroup;
            } else {
                BG_T;
                long conversation = Game->AddNewConversation(Me, false, false);
                Game->AddPersonToConversation(conversation, Game->GetHero());
                Game->AddLineToConversation(conversation, BG_TEXT("MADE_REDUNDANT"), false, Me, Game->GetHero());
                while (Game->IsConversationActive(conversation)) {
                    Game->NewScriptFrame();
                    BG_T;
                }
                BG_T;
                SetHenchmanAsHired(false);
                break;
            }
        }
        if (BrainState == 3) {
            BG_T;
            SetHenchmanAsHired(false);
        }
    }
}
