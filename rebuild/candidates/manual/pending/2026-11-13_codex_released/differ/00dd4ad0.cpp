// NScript::CQ_PrisonEscapeRescueMotherScript::CPrisonerAccomplice::Main() -- the cell-mate prisoner: until released, whenever the hero talks to him play a paused speech scene (key stolen: KEY; race won: HINT2; more than one race run: HINT3; else HINT4_01 + HINT4_02, each line only while he is alive); once the hero uses his cell door, give the KEY line, free him (persistent, no information, morality reward) and follow the hero, joining the prisoner faction: until the courtyard region is loaded comment COURTYARD every 30 s and complain when the hero hits him, then stop region-following and comment GUARDS / complain until the thread ends
// Grade: DIFFER(3467v3467) -- only the predicate block: retail expands ~CIsThingAlive (CIsThingAlive vptr 0x012C32F0 held in ebx, stored again before the out-of-line ~CScriptThing 0x004AA840, so ebx is pushed in the prologue); ours calls ~CIsThingAlive 0x00E241B0 (plain inline: VC7.1's per-site budget) or expands ~CScriptThing too (__forceinline / implicit dtor); the size is exact and the rest is the loop-alignment pad shift that follows
// Next idea: a retail-plausible way to make the TU's ~CScriptThing cost cross VC7.1's f=1 inline threshold while staying below it at the inner-loop exit 0x00DD5385 (where retail expands it); the sibling CPrisonGuard::Main 0x00DD44C0 (same TU) shows the same expanded ~CIsThingAlive / called ~CScriptThing predicate
// Leaf: ?Main@CPrisonerAccomplice@CQ_PrisonEscapeRescueMotherScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DD4AD0, 3467 bytes, plain `ret` (four epilogues). Identity: vtable 0x012DBEF0 slot 1 (Alloc
//   0x00DD49A0), Ego_r.pdb / FableWin.pdb ?Main@CPrisonerAccomplice@CQ_PrisonEscapeRescueMotherScript@NScript@@EAEXXZ.
// Callees: CScriptThing copy 0x004ABE90 / ~CScriptThing 0x004AA840 (expanded at the cutscene-wait exit 0x00DD5385: ~CBaseClass
//   0x0099A2E0), SetPredicate 0x00F35B10, IsActiveThreadTerminating 0x00F35B30, CBaseObject() 0x0099A380 / ~CBaseObject 0x0099A430,
//   scripted-thing handle IsValid 0x00CD23B9 / Release 0x00CD2770 / Speak(const char*) 0x007E7390 / IsPerformingScriptTask 0x007E7450 /
//   GetScriptThing 0x007E7490 / dtor COMDAT 0x007E74D0, movie dtor COMDAT 0x006E7B80 / ~CCountedPointer<MovieBase> 0x006E7AB0,
//   ~CTimer 0x00CD4470 (pDSTGame 0x0143E8F8 slots 87-90 where expanded), MakePrisonerComment 0x00DD5860, CCharString 0x0099EBF0 /
//   0x0099EAE0, operator delete 0x00BFE9BC. Interface (+0x04) slots 7, 8, 12 IsRegionLoaded, 70, 77 GetNearestWithScriptName, 156
//   GiveHeroMorality, 229, 235, 264 GetHealth, 361, 364-366, 370, 379, 477, 480, 601; CScriptThing slots 21, 27 MsgIsTalkedToBy, 41, 42
//   on Me; slot 20 MsgIsUsedBy through the cell door's PImp (the local is devirtualised: inline if-form wrapper).
// Strings: "CellDoor", "SCRIPT_NAME_HERO", "", "TEXT_QST_055_PRISON_PRISONER_KEY" / _HINT2 / _HINT3 / _HINT4_01 / _HINT4_02,
//   "FACTION_PRISONER", "PrisonCourtyard", "COURTYARD", "TEXT_QST_055_PRISON_PRISONER_ON_HIT", "GUARDS". Data: pGlobalScriptDef
//   0x0143E90C +0xDE8 PrisonFreePrisonerMorality; the health threshold 0.0f at 0x0122DEDC.
// Layout: Ego_r.pdb CPrisonerAccomplice: CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14 (PrisonCutsceneFinished +0x6A),
//   PMasterData +0x18 (CQ_SunnyvaleMasterData: PrisonRaceNumber +0xE8, PrisonRaceWonByHero +0xEC, PrisonKeyStolenByHero +0xEE),
//   ReleasedAndFollowing +0x1C. The class is declared in engine/CQ_PrisonEscapeRescueMotherScript.h and defined here.
// Shape: the cutscene-wait / talk / cell-door body is a `while (!ReleasedAndFollowing)` loop (rotated: the release branch follows it);
//   every exit inside the movie block is `{ game->PauseAllNonScriptedEntities(false); return; }` through a `game = Game` local (ebp);
//   the temporary-dtor flag word for the hit chains is the entry zero store at +0x28; row-local inline ~CScriptThing and the
//   implicit STB / movie dtors give retail's expanded / called mix at every other site.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRGBCOLOUR
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CIsThingAlive.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();   // 0x006E7AB0
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CQ_PrisonEscapeRescueMotherScript.h"
#include "engine/CScriptDef.h"
#include "engine/CDefPointer.h"
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
inline NScript::CIsThingAlive::~CIsThingAlive() { }
inline CScriptThing::~CScriptThing() { }
inline bool CScriptThing::MsgIsUsedBy(const CCharString& name) const { if (!PImp.Data) return false; return PImp.Data->MsgIsUsedBy(name); }
extern CGameScriptInterfaceBase* pDSTGame;                       // 0x0143E8F8
inline CTimer::CTimer() { TimerIndex = pDSTGame->RegisterTimer(); }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum EHeroAbility { };
enum ETextGroupSelectionMethod { };
extern CDefPointer<const CScriptDef> pGlobalScriptDef;           // 0x0143E90C

namespace NScript {
class CQ_PrisonEscapeRescueMotherScript::CPrisonerAccomplice : public CEntityScriptBase {
public:
    CQ_PrisonEscapeRescueMotherScript* ParentClass;              // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                         // +0x18
    bool ReleasedAndFollowing;                                   // +0x1c
private:
    virtual void Main();
};
}

void NScript::CQ_PrisonEscapeRescueMotherScript::CPrisonerAccomplice::Main()
{
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    CTimer comment_timer;
    ParentClass->PrisonCutsceneFinished = false;
    while (!ParentClass->PrisonCutsceneFinished) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptThing cell_door = Game->GetNearestWithScriptName(Me, "CellDoor");
    while (!ReleasedAndFollowing) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        while (!ParentClass->PrisonCutsceneFinished) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                return;
            if (seh_me.IsValid())
                seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            CScriptGameResourceObjectMovieBase movie;
            Game->StartMovieSequence("", movie);
            CGameScriptInterfaceBase* game = Game;
            game->PauseAllNonScriptedEntities(true);
            if (PMasterData->PrisonKeyStolenByHero) {
                if (IsActiveThreadTerminating()) { game->PauseAllNonScriptedEntities(false); return; }
                if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game->GetHero(), "TEXT_QST_055_PRISON_PRISONER_KEY", (ETextGroupSelectionMethod)0, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) { game->PauseAllNonScriptedEntities(false); return; }
                    }
                    if (IsActiveThreadTerminating()) { game->PauseAllNonScriptedEntities(false); return; }
                }
            } else if (PMasterData->PrisonRaceWonByHero) {
                if (IsActiveThreadTerminating()) { game->PauseAllNonScriptedEntities(false); return; }
                if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game->GetHero(), "TEXT_QST_055_PRISON_PRISONER_HINT2", (ETextGroupSelectionMethod)0, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) { game->PauseAllNonScriptedEntities(false); return; }
                    }
                    if (IsActiveThreadTerminating()) { game->PauseAllNonScriptedEntities(false); return; }
                }
            } else if (PMasterData->PrisonRaceNumber > 1) {
                if (IsActiveThreadTerminating()) { game->PauseAllNonScriptedEntities(false); return; }
                if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game->GetHero(), "TEXT_QST_055_PRISON_PRISONER_HINT3", (ETextGroupSelectionMethod)0, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) { game->PauseAllNonScriptedEntities(false); return; }
                    }
                    if (IsActiveThreadTerminating()) { game->PauseAllNonScriptedEntities(false); return; }
                }
            } else {
                if (IsActiveThreadTerminating()) { game->PauseAllNonScriptedEntities(false); return; }
                if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game->GetHero(), "TEXT_QST_055_PRISON_PRISONER_HINT4_01", (ETextGroupSelectionMethod)0, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) { game->PauseAllNonScriptedEntities(false); return; }
                    }
                    if (IsActiveThreadTerminating()) { game->PauseAllNonScriptedEntities(false); return; }
                }
                if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game->GetHero(), "TEXT_QST_055_PRISON_PRISONER_HINT4_02", (ETextGroupSelectionMethod)0, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) { game->PauseAllNonScriptedEntities(false); return; }
                    }
                    if (IsActiveThreadTerminating()) { game->PauseAllNonScriptedEntities(false); return; }
                }
            }
            if (seh_me.IsValid())
                seh_me.Release();
            game->PauseAllNonScriptedEntities(false);
        }
        if (cell_door.MsgIsUsedBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                return;
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            Game->AddLineToConversation(conversation, "TEXT_QST_055_PRISON_PRISONER_KEY", false, Me, Game->GetHero());
            Game->SetThingPersistent(Me, true);
            Game->ClearThingHasInformation(Me);
            Game->GiveHeroMorality(pGlobalScriptDef->PrisonFreePrisonerMorality);
            ReleasedAndFollowing = true;
        }
    }
    if (IsActiveThreadTerminating())
        return;
    if (seh_me.IsValid())
        seh_me.Release();
    Game->EntitySetAsAllowedToFollowHero(Me, true);
    Game->EntityFollowThing(Me, Game->GetHero(), 3.0f, true);
    Game->SetEntityAsRegionFollowing(Me, Game->GetHero(), true);
    Game->SetThingPersistent(Me, true);
    Game->EntitySetInFaction(Me, "FACTION_PRISONER");
    while (!Game->IsRegionLoaded("PrisonCourtyard")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (comment_timer.GetTime() == 0 && ParentClass->MakePrisonerComment(Me, "COURTYARD")) {
            if (IsActiveThreadTerminating())
                return;
            comment_timer = 30;
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO")
            || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom((EHeroAbility)14, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating())
                return;
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            Game->AddLineToConversation(conversation, "TEXT_QST_055_PRISON_PRISONER_ON_HIT", false, Me, Game->GetHero());
        }
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetEntityAsRegionFollowing(Me, Game->GetHero(), false);
    while (!IsActiveThreadTerminating()) {
        if (comment_timer.GetTime() == 0 && ParentClass->MakePrisonerComment(Me, "GUARDS")) {
            if (IsActiveThreadTerminating())
                return;
            comment_timer = 30;
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO")
            || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom((EHeroAbility)14, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating())
                return;
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            Game->AddLineToConversation(conversation, "TEXT_QST_055_PRISON_PRISONER_ON_HIT", false, Me, Game->GetHero());
        }
        Game->NewScriptFrame();
    }
}
