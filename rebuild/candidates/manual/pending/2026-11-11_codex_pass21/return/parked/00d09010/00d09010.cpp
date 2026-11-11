// NScript::CQ_BanditCampBossBattleScript::CCampHostageGuard::Main() -- the bandit-camp hostage guard: scripts himself, faces the hostage and, until the hostages are rescued, chats to the hostage when the hero is near, paces between his two markers, answers the hero once (first chat, then second), and turns on the hero when struck; afterwards he paces at his first marker, taunts the hero (the sixth late line makes him attack), answers talk and turns on the hero when struck
// Grade: DIFFER(5672v5864) -- Missing native hot Timer cleanup copies, remaining temporary Boolean/CSE and stack-slot differences, and switch-table operands. 16 strict / 10 loose hunks; 13 original jump-table words retained.
// Inline-budget: S15 plus forced original Movie ctor selects implicit Movie cleanup; visible called Timer destructor preserves native frame. Hot Timer cleanup copies still differ.
// Flags: /d2inlS15
// Leaf: ?Main@CCampHostageGuard@CQ_BanditCampBossBattleScript@NScript@@EAEXXZ
// Retail: 0x00D09010, 5864 bytes, thiscall.
// Identity (proven, two sources): Ego_r.pdb and FableWin.pdb name sets hold ?Main@CCampHostageGuard@CQ_BanditCampBossBattleScript@NScript@@EAEXXZ;
//   one .rdata reference, slot 1 of the entity vtable 0x012C9130 stored by the landed CCampHostageGuard::Alloc 0x00D08E50.
// Layout: engine/CQ_BanditCampBossBattleScript.h with the chunk 812 CAMPHOSTAGEGUARD opt-in (hdr/README.md).
#define FABLE_RETAIL_CPREDICATEBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CISTHINGALIVE_ORIGINAL_ACCESS
#define FABLE_RETAIL_CSCRIPTRESOURCE_ORIGINAL_ACCESS
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CBASEOBJECT_ORIGINAL_ACCESS
#define FABLE_RETAIL_CSCRIPTTHING_ORIGINAL_ACCESS
#define FABLE_RETAIL_CTIMER_ORIGINAL_ACCESS
#define FABLE_RETAIL_CQ_BANDITCAMPBOSSBATTLESCRIPT_CAMPHOSTAGEGUARD
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_FORCEINLINE_CTOR
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRGBCOLOUR
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_BanditCampBossBattleScript.h"
#include "engine/CIsThingAlive.h"
#include "engine/CRGBColour.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
class CScriptGameResourceObjectMovieBase;
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CTimer.h"
#include "engine/TRetVoid.h"
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& t):Thing(t) {}
inline NScript::CIsThingAlive::~CIsThingAlive() { }
bool FABLE_FASTCALL IsDistanceBetweenThingsUnder(const CScriptThing&, const CScriptThing&, float);   // 0x00CBE2FF
extern CGameScriptInterfaceBase* pDSTGame;                              // 0x0143E8F8
inline CTimer::CTimer():TimerIndex(pDSTGame->RegisterTimer()) {}
#ifdef _MSC_VER
#pragma auto_inline(off)
#endif
CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
#ifdef _MSC_VER
#pragma auto_inline(on)
#endif
FABLE_FORCEINLINE CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex,time); return *this; }
FABLE_FORCEINLINE long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
bool FABLE_FASTCALL IsDistanceBetweenThingsOver(const CScriptThing&, const CScriptThing&, float);    // 0x00CBE3EA
#include "engine/EHeroAbility.h"

void NScript::CQ_BanditCampBossBattleScript::CCampHostageGuard::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid())
        seh_me.Release();
    while (!Game.StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    CTimer GoadingTimer;
    GoadingTimer = 0;
    long GoadLevel = 0;
    bool SpokenTo = false;
    CScriptThing Hostage = Game.GetNearestWithScriptName(Me, "CampHostage");
    Game.EntitySetFacingAngleTowardsThing(Me, Hostage, false);
    long MarkerAt = 1;
    bool Turning = false;
    CTimer TurnTimer;
    DropPass = true;
    while (!ParentClass.HostagesRescued) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        if (Game.MsgOnHeroPickedPocket(Me)) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            DropPass = false;
        }
        if (IsDistanceBetweenThingsUnder(Me, Game.GetHero(), 7.0f) && IsDistanceBetweenThingsUnder(Me, Hostage, 12.0f) && GoadingTimer.GetTime() <= 0) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            long Conversation = Game.AddNewConversation(Me, false, false);
            Game.AddPersonToConversation(Conversation, Hostage);
            switch (GoadLevel) {
            case 0:
                Game.AddLineToConversation(Conversation, "TEXT_QST_009_HOSTAGE_GUARD_FIRST", false, Me, Hostage);
                GoadLevel++;
                break;
            case 1:
                Game.AddLineToConversation(Conversation, "TEXT_QST_009_HOSTAGE_GUARD_SECOND", false, Me, Hostage);
                GoadLevel++;
                break;
            case 2:
                Game.AddLineToConversation(Conversation, "TEXT_QST_009_HOSTAGE_GUARD_THIRD", false, Me, Hostage);
                GoadLevel++;
                break;
            case 3:
                Game.AddLineToConversation(Conversation, "TEXT_QST_009_HOSTAGE_GUARD_FOURTH", false, Me, Hostage);
                GoadLevel++;
                break;
            case 4:
                Game.AddLineToConversation(Conversation, "TEXT_QST_009_HOSTAGE_GUARD_FIFTH", false, Me, Hostage);
                GoadLevel++;
                break;
            case 5:
                Game.AddLineToConversation(Conversation, "TEXT_QST_009_HOSTAGE_GUARD_SIXTH", false, Me, Hostage);
                GoadLevel = 6;
                break;
            case 6:
                Game.AddLineToConversation(Conversation, "TEXT_QST_009_HOSTAGE_GUARD_SEVENTH", false, Me, Hostage);
                GoadLevel = 5;
                break;
            }
            GoadingTimer = 10;
        }
        if (MarkerAt == 1) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            if (IsDistanceBetweenThingsOver(Me, Game.GetThingWithScriptName("GuardFirstMarker"), 2.0f) && !seh_me.IsPerformingScriptTask()) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                seh_me.MoveToPosition(Game.GetThingWithScriptName("GuardFirstMarker").GetPos(), 1.0f, (EScriptEntityMoveType)0, false, true);
            } else {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                if (TurnTimer.GetTime() <= 0) {
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    if (!Turning) {
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                        Turning = true;
                        TurnTimer = 10;
                    } else {
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                        Turning = false;
                        MarkerAt = 2;
                    }
                }
            }
        } else {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            if (IsDistanceBetweenThingsOver(Me, Game.GetThingWithScriptName("GuardSecondMarker"), 2.0f) && !seh_me.IsPerformingScriptTask()) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                seh_me.MoveToPosition(Game.GetThingWithScriptName("GuardSecondMarker").GetPos(), 1.0f, (EScriptEntityMoveType)0, false, true);
            } else {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                if (TurnTimer.GetTime() <= 0) {
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    if (!Turning) {
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                        Turning = true;
                        TurnTimer = 10;
                    } else {
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                        Turning = false;
                        MarkerAt = 1;
                    }
                }
            }
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            seh_me.ClearCommands();
            if (!SpokenTo) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game.GetHero(), "TEXT_QST_009_HOSTAGE_GUARD_FIRST_CHAT", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
                Game.EntitySetFacingAngleTowardsThing(Me, Hostage, false);
                SpokenTo = true;
            } else {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game.GetHero(), "TEXT_QST_009_HOSTAGE_GUARD_SECOND_CHAT", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
                Game.EntitySetFacingAngleTowardsThing(Me, Hostage, false);
            }
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom((EHeroAbility)HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO")) || ParentClass.HostageKilled) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            Game.ClearThingHasInformation(Me);
            Game.GiveThingBestEnemyTarget(Me, Game.GetHero());
            if (seh_me.IsValid())
                seh_me.Release();
            while (true) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    SpokenTo = false;
    Game.ClearThingHasInformation(Me);
    for (; !false; Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        if (IsDistanceBetweenThingsOver(Me, Game.GetThingWithScriptName("GuardFirstMarker"), 2.0f) && !seh_me.IsPerformingScriptTask()) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            seh_me.MoveToPosition(Game.GetThingWithScriptName("GuardFirstMarker").GetPos(), 1.0f, (EScriptEntityMoveType)0, false, true);
        }
        if (IsDistanceBetweenThingsUnder(Me, Game.GetHero(), 7.0f) && IsDistanceBetweenThingsUnder(Me, Game.GetThingWithScriptName("GuardFirstMarker"), 12.0f) && GoadingTimer.GetTime() <= 0) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            long Conversation = Game.AddNewConversation(Me, false, false);
            Game.AddPersonToConversation(Conversation, Game.GetHero());
            switch (GoadLevel) {
            case 0:
                Game.AddLineToConversation(Conversation, "TEXT_QST_009_HOSTAGE_GUARD_LATE_CONVO_FIRST", false, Me, Game.GetHero());
                GoadLevel++;
                break;
            case 1:
                Game.AddLineToConversation(Conversation, "TEXT_QST_009_HOSTAGE_GUARD_LATE_CONVO_SECOND", false, Me, Game.GetHero());
                GoadLevel++;
                break;
            case 2:
                Game.AddLineToConversation(Conversation, "TEXT_QST_009_HOSTAGE_GUARD_LATE_CONVO_THIRD", false, Me, Game.GetHero());
                GoadLevel++;
                break;
            case 3:
                Game.AddLineToConversation(Conversation, "TEXT_QST_009_HOSTAGE_GUARD_LATE_CONVO_FOURTH", false, Me, Game.GetHero());
                GoadLevel++;
                break;
            case 4:
                Game.AddLineToConversation(Conversation, "TEXT_QST_009_HOSTAGE_GUARD_LATE_CONVO_FIFTH", false, Me, Game.GetHero());
                GoadLevel++;
                break;
            case 5:
                Game.AddLineToConversation(Conversation, "TEXT_QST_009_HOSTAGE_GUARD_LATE_CONVO_SIXTH", false, Me, Game.GetHero());
                Game.GiveThingBestEnemyTarget(Me, Game.GetHero());
                if (seh_me.IsValid())
                    seh_me.Release();
                while (true) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            GoadingTimer = 10;
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            seh_me.ClearCommands();
            if (!SpokenTo) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game.GetHero(), "TEXT_QST_009_HOSTAGE_GUARD_LATE_SPEAK_FIRST", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
                SpokenTo = true;
            } else {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game.GetHero(), "TEXT_QST_009_HOSTAGE_GUARD_LATE_SPEAK_SECOND", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
            }
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom((EHeroAbility)HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            seh_me.ClearCommands();
            {
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game.GetHero(), "TEXT_QST_009_HOSTAGE_GUARD_LATE_HIT", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
                Game.GiveThingBestEnemyTarget(Me, Game.GetHero());
                if (seh_me.IsValid())
                    seh_me.Release();
            }
            while (true) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}
