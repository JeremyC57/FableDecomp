// NScript::CQ_BanditCampBossBattleScript::CCampHostageGuard::Main() -- the bandit-camp hostage guard: scripts himself, faces the hostage and, until the hostages are rescued, chats to the hostage when the hero is near, paces between his two markers, answers the hero once (first chat, then second), and turns on the hero when struck; afterwards he paces at his first marker, taunts the hero (the sixth late line makes him attack), answers talk and turns on the hero when struck
// Grade: DIFFER(5880v5864) -- 170 lines: MOVIEBASE_FORCEINLINE expands the movie dtors with the MovieBase vptr store retail lacks (9 x 8 B; the implicit dtor + declared-only PImp spec calls the COMDAT instead: 664); retail expands ~CTimer at the attacked-wait exit too (DeregisterTimer x2, ~Hostage, ~seh_me, ret) where ours shares the epilogue; one alignment pad; eax/edx colouring. Chunk 819 a3: TalkedTo = false written before ClearThingHasInformation now stores al (the thread-check result) into the right byte as retail; it is scheduled before the vtable load where retail stores after the push
// Next idea: the movie-dtor expansion without the vptr reset (no spelling found: user-declared forceinline adds the store, implicit is called); the store schedule (store after push edi)
// Leaf: ?Main@CCampHostageGuard@CQ_BanditCampBossBattleScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D09010, 5864 bytes (incl. two switch tables), thiscall, plain `ret`, no EH frame.
// Identity (proven, two sources): Ego_r.pdb and FableWin.pdb name sets hold ?Main@CCampHostageGuard@CQ_BanditCampBossBattleScript@NScript@@EAEXXZ;
//   one .rdata reference, slot 1 of the entity vtable 0x012C9130 stored by the landed CCampHostageGuard::Alloc 0x00D08E50.
// Layout: engine/CQ_BanditCampBossBattleScript.h with the chunk 812 CAMPHOSTAGEGUARD opt-in (hdr/README.md).
#define FABLE_RETAIL_CQ_BANDITCAMPBOSSBATTLESCRIPT_CAMPHOSTAGEGUARD
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_FORCEINLINE
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
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& t):Thing(t) {}
inline NScript::CIsThingAlive::~CIsThingAlive() { }
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing&, const CScriptThing&, float);   // 0x00CBE2FF
extern CGameScriptInterfaceBase* pDSTGame;                              // 0x0143E8F8
inline CTimer::CTimer():TimerIndex(pDSTGame->RegisterTimer()) {}
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex,time); return *this; }
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
bool __fastcall IsDistanceBetweenThingsOver(const CScriptThing&, const CScriptThing&, float);    // 0x00CBE3EA
enum { HERO_ABILITY_HEAL_LIFE_SPELL = 14 };

void NScript::CQ_BanditCampBossBattleScript::CCampHostageGuard::Main()
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
    CTimer ChatTimer;
    ChatTimer = 0;
    long ChatCount = 0;
    bool TalkedTo = false;
    CScriptThing Hostage = Game->GetNearestWithScriptName(Me, "CampHostage");
    Game->EntitySetFacingAngleTowardsThing(Me, Hostage, false);
    long MarkerState = 1;
    bool Waited = false;
    CTimer MoveTimer;
    DropPass = true;
    while (!ParentClass.HostagesRescued) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (Game->MsgOnHeroPickedPocket(Me)) {
            if (IsActiveThreadTerminating())
                return;
            DropPass = false;
        }
        if (IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 7.0f) && IsDistanceBetweenThingsUnder(Me, Hostage, 12.0f) && ChatTimer.GetTime() <= 0) {
            if (IsActiveThreadTerminating())
                return;
            long ConversationIndex = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(ConversationIndex, Hostage);
            switch (ChatCount) {
            case 0:
                Game->AddLineToConversation(ConversationIndex, "TEXT_QST_009_HOSTAGE_GUARD_FIRST", false, Me, Hostage);
                ChatCount = 1;
                break;
            case 1:
                Game->AddLineToConversation(ConversationIndex, "TEXT_QST_009_HOSTAGE_GUARD_SECOND", false, Me, Hostage);
                ChatCount = 2;
                break;
            case 2:
                Game->AddLineToConversation(ConversationIndex, "TEXT_QST_009_HOSTAGE_GUARD_THIRD", false, Me, Hostage);
                ChatCount = 3;
                break;
            case 3:
                Game->AddLineToConversation(ConversationIndex, "TEXT_QST_009_HOSTAGE_GUARD_FOURTH", false, Me, Hostage);
                ChatCount = 4;
                break;
            case 4:
                Game->AddLineToConversation(ConversationIndex, "TEXT_QST_009_HOSTAGE_GUARD_FIFTH", false, Me, Hostage);
                ChatCount = 5;
                break;
            case 5:
                Game->AddLineToConversation(ConversationIndex, "TEXT_QST_009_HOSTAGE_GUARD_SIXTH", false, Me, Hostage);
                ChatCount = 6;
                break;
            case 6:
                Game->AddLineToConversation(ConversationIndex, "TEXT_QST_009_HOSTAGE_GUARD_SEVENTH", false, Me, Hostage);
                ChatCount = 5;
                break;
            }
            ChatTimer = 10;
        }
        if (MarkerState == 1) {
            if (IsActiveThreadTerminating())
                return;
            if (IsDistanceBetweenThingsOver(Me, Game->GetThingWithScriptName("GuardFirstMarker"), 2.0f) && !seh_me.IsPerformingScriptTask()) {
                if (IsActiveThreadTerminating())
                    return;
                seh_me.MoveToPosition(Game->GetThingWithScriptName("GuardFirstMarker").GetPos(), 1.0f, (EScriptEntityMoveType)0, false, true);
            } else {
                if (IsActiveThreadTerminating())
                    return;
                if (MoveTimer.GetTime() <= 0) {
                    if (IsActiveThreadTerminating())
                        return;
                    if (!Waited) {
                        if (IsActiveThreadTerminating())
                            return;
                        Waited = true;
                        MoveTimer = 10;
                    } else {
                        if (IsActiveThreadTerminating())
                            return;
                        MarkerState = 2;
                        Waited = false;
                    }
                }
            }
        } else {
            if (IsActiveThreadTerminating())
                return;
            if (IsDistanceBetweenThingsOver(Me, Game->GetThingWithScriptName("GuardSecondMarker"), 2.0f) && !seh_me.IsPerformingScriptTask()) {
                if (IsActiveThreadTerminating())
                    return;
                seh_me.MoveToPosition(Game->GetThingWithScriptName("GuardSecondMarker").GetPos(), 1.0f, (EScriptEntityMoveType)0, false, true);
            } else {
                if (IsActiveThreadTerminating())
                    return;
                if (MoveTimer.GetTime() <= 0) {
                    if (IsActiveThreadTerminating())
                        return;
                    if (!Waited) {
                        if (IsActiveThreadTerminating())
                            return;
                        Waited = true;
                        MoveTimer = 10;
                    } else {
                        if (IsActiveThreadTerminating())
                            return;
                        MarkerState = 1;
                        Waited = false;
                    }
                }
            }
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                return;
            seh_me.ClearCommands();
            if (!TalkedTo) {
                if (IsActiveThreadTerminating())
                    return;
                CScriptGameResourceObjectMovieBase movie;
                Game->StartMovieSequence("", movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game->GetHero(), "TEXT_QST_009_HOSTAGE_GUARD_FIRST_CHAT", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return;
                    }
                    if (IsActiveThreadTerminating())
                        return;
                }
                Game->EntitySetFacingAngleTowardsThing(Me, Hostage, false);
                TalkedTo = true;
            } else {
                if (IsActiveThreadTerminating())
                    return;
                CScriptGameResourceObjectMovieBase movie;
                Game->StartMovieSequence("", movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game->GetHero(), "TEXT_QST_009_HOSTAGE_GUARD_SECOND_CHAT", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return;
                    }
                    if (IsActiveThreadTerminating())
                        return;
                }
                Game->EntitySetFacingAngleTowardsThing(Me, Hostage, false);
            }
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom((EHeroAbility)HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO")) || ParentClass.HostageKilled) {
            if (IsActiveThreadTerminating())
                return;
            Game->ClearThingHasInformation(Me);
            Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
            if (seh_me.IsValid())
                seh_me.Release();
            for (;;) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
        }
    }
    if (IsActiveThreadTerminating())
        return;
    TalkedTo = false;
    Game->ClearThingHasInformation(Me);
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        if (IsDistanceBetweenThingsOver(Me, Game->GetThingWithScriptName("GuardFirstMarker"), 2.0f) && !seh_me.IsPerformingScriptTask()) {
            if (IsActiveThreadTerminating())
                return;
            seh_me.MoveToPosition(Game->GetThingWithScriptName("GuardFirstMarker").GetPos(), 1.0f, (EScriptEntityMoveType)0, false, true);
        }
        if (IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 7.0f) && IsDistanceBetweenThingsUnder(Me, Game->GetThingWithScriptName("GuardFirstMarker"), 12.0f) && ChatTimer.GetTime() <= 0) {
            if (IsActiveThreadTerminating())
                return;
            long ConversationIndex = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(ConversationIndex, Game->GetHero());
            switch (ChatCount) {
            case 0:
                Game->AddLineToConversation(ConversationIndex, "TEXT_QST_009_HOSTAGE_GUARD_LATE_CONVO_FIRST", false, Me, Game->GetHero());
                ChatCount = 1;
                break;
            case 1:
                Game->AddLineToConversation(ConversationIndex, "TEXT_QST_009_HOSTAGE_GUARD_LATE_CONVO_SECOND", false, Me, Game->GetHero());
                ChatCount = 2;
                break;
            case 2:
                Game->AddLineToConversation(ConversationIndex, "TEXT_QST_009_HOSTAGE_GUARD_LATE_CONVO_THIRD", false, Me, Game->GetHero());
                ChatCount = 3;
                break;
            case 3:
                Game->AddLineToConversation(ConversationIndex, "TEXT_QST_009_HOSTAGE_GUARD_LATE_CONVO_FOURTH", false, Me, Game->GetHero());
                ChatCount = 4;
                break;
            case 4:
                Game->AddLineToConversation(ConversationIndex, "TEXT_QST_009_HOSTAGE_GUARD_LATE_CONVO_FIFTH", false, Me, Game->GetHero());
                ChatCount = 5;
                break;
            case 5:
                Game->AddLineToConversation(ConversationIndex, "TEXT_QST_009_HOSTAGE_GUARD_LATE_CONVO_SIXTH", false, Me, Game->GetHero());
                Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
                if (seh_me.IsValid())
                    seh_me.Release();
                for (;;) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
            }
            ChatTimer = 10;
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                return;
            seh_me.ClearCommands();
            if (!TalkedTo) {
                if (IsActiveThreadTerminating())
                    return;
                CScriptGameResourceObjectMovieBase movie;
                Game->StartMovieSequence("", movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game->GetHero(), "TEXT_QST_009_HOSTAGE_GUARD_LATE_SPEAK_FIRST", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return;
                    }
                    if (IsActiveThreadTerminating())
                        return;
                }
                TalkedTo = true;
            } else {
                if (IsActiveThreadTerminating())
                    return;
                CScriptGameResourceObjectMovieBase movie;
                Game->StartMovieSequence("", movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game->GetHero(), "TEXT_QST_009_HOSTAGE_GUARD_LATE_SPEAK_SECOND", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return;
                    }
                    if (IsActiveThreadTerminating())
                        return;
                }
            }
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom((EHeroAbility)HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating())
                return;
            seh_me.ClearCommands();
            {
                CScriptGameResourceObjectMovieBase movie;
                Game->StartMovieSequence("", movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game->GetHero(), "TEXT_QST_009_HOSTAGE_GUARD_LATE_HIT", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return;
                    }
                    if (IsActiveThreadTerminating())
                        return;
                }
                Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
                if (seh_me.IsValid())
                    seh_me.Release();
            }
            for (;;) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
        }
        Game->NewScriptFrame();
    }
}
