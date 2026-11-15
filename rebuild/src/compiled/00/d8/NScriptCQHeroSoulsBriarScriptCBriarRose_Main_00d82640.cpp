// NScript::CQ_HeroSoulsBriarScript::CBriarRose::Main() -- Briar Rose in the Hero Souls quest: take her over, play the soul-2 intro cut-scene once (line chosen by KilledThunder), offer information and the HUD marker, answer talks (two first lines, then sequential), turn ally when hit away from the shrine; when hit within 13 m of HS_MiddleOfShrine she speaks, starts Q_HeroSoulsBriarMagicBarrier and becomes the boss with a taunt timer until her health drops to 5
// Inline-budget: #pragma inline_depth(0) round the row-local inline CIsThingAlive ctor and dtor definitions + FABLE_RETAIL_CPREDICATEBASE_NOVTABLE
//   (the landed predicate lever of chunk H40, as landed CFishermansWife::Main 0x00D8BCD0) -- retail expands the predicate ctor / dtor one
//   level and CALLS the nested CScriptThing copy 0x004ABE90 / ~CScriptThing 0x004AA840, while the by-value CScriptThing copies of the same
//   function expand; with plain inline VC7.1 calls ~CIsThingAlive whole or expands the nested members too (GOTCHAS chunks 799/810).
// Shape (EVIDENCED, landed twin CHeroThunder::Main 0x00D7B510, same quest family): the closing idle loop is `if (T) return; for (;;) { NSF;
//   if (T) return; }` -- VC7.1 tail-merges the pre-loop test into the loop test, so the health loop exit jumps to the single bottom test
//   (retail 0x00D832C4 `jmp 0x00D832CE`); `while (!T) NSF;` (and for/do/break spellings) duplicate the test at the loop head (+10 bytes).
// Leaf: ?Main@CBriarRose@CQ_HeroSoulsBriarScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D82640, 3260 bytes, thiscall, plain `ret`, no EH frame; ends at 0x00D832FB.
// Identity (proven, two sources): Ego_r.pdb / FableWin.pdb name sets hold ?Main@CBriarRose@CQ_HeroSoulsBriarScript@NScript@@EAEXXZ; the
//   address has one .rdata reference, slot 1 (0x012D42FC) of the CBriarRose entity vtable 0x012D42F8 stored by the landed CBriarRose::Alloc
//   0x00D81630 (Init 0x00D816E0, GetParentScript 0x00D816D0, OnPredicateFail 0x00D816F0 landed).
#define FABLE_RETAIL_CPREDICATEBASE_NOVTABLE
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CQ_HeroSoulsBriarScript.h"
#include "engine/CIsThingAlive.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CRGBColour.h"
#include "engine/CTimer.h"
#include "engine/EHeroAbility.h"
#include "engine/ETextGroupSelectionMethod.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
extern CGameScriptInterfaceBase* pDSTGame;               // 0x0143E8F8
FABLE_FORCEINLINE CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
FABLE_FORCEINLINE long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
inline CScriptThing::~CScriptThing() { }
#pragma inline_depth(0)
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
#pragma inline_depth()
#pragma inline_depth(0)
inline NScript::CIsThingAlive::~CIsThingAlive() {}
#pragma inline_depth()
bool FABLE_FASTCALL IsDistanceBetweenThingsUnder(const CScriptThing&, const CScriptThing&, float);   // 0x00CBE2FF
CCharString FABLE_FASTCALL GFIntToCharString(long);                                                 // 0x0099F830
CCharString FABLE_FASTCALL operator+(const char*, const CCharString&);                              // 0x0099F690

void NScript::CQ_HeroSoulsBriarScript::CBriarRose::Main()
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
    ParentClass.seh_BriarRose = seh_me;
    Game->SetIsPushableByHero(Me, false);
    if (!ParentClass.PlayedBriarCS) {
        if (IsActiveThreadTerminating()) return;
        if (PMasterData->KilledThunder) {
            if (IsActiveThreadTerminating()) return;
            ParentClass.csargs["$LINE1"] = "TEXT_CS_B04_SOUL2_INTRO_10B";
        } else {
            if (IsActiveThreadTerminating()) return;
            ParentClass.csargs["$LINE1"] = "TEXT_CS_B04_SOUL2_INTRO_10";
        }
        ParentClass.PlayCutscene("CS_HEROSOULS_SOUL2_INTRO");
        ParentClass.PlayedBriarCS = true;
        PMasterData->CurrentHeroSoulsPosition = EPIHS_2ND_SOUL_VISIT_YOUR_MOTHER;
        ++PMasterData->DeliveredSoul;
    }
    Game->SetThingHasInformation(Me, false, false, false);
    Game->MiniMapAddMarker(Me, "HUD_ORB_QUEST_CORE");
    Game->EntitySetAlwaysBlockAttacksFromThing(Me, Game->GetHero(), true);
    Game->EntitySetAsKillable(Me, false, false);
    Game->EntitySetAsToAddToComboMultiplierWhenHit(Me, false);
    Game->EntitySetAsDamageable(Me, false);
    CScriptThing shrine = Game->GetThingWithScriptName("HS_MiddleOfShrine");
    if (IsActiveThreadTerminating()) return;
    for (;;) {
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (!FirstSpeakAndWaitDone) {
                if (IsActiveThreadTerminating()) return;
                if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game->GetHero(), "TEXT_QST_B04_BRIAR_SECONDSOUL_ONTALK_10", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                }
                if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game->GetHero(), "TEXT_QST_B04_BRIAR_SECONDSOUL_ONTALK_20", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                }
                FirstSpeakAndWaitDone = true;
            } else {
                if (IsActiveThreadTerminating()) return;
                if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game->GetHero(), "TEXT_QST_B04_BRIAR_SECONDSOUL_ONTALK", GROUP_SELECT_SEQUENTIAL, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                }
            }
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return;
            if (IsDistanceBetweenThingsUnder(Game->GetHero(), shrine, 13.0f)) break;
            if (IsActiveThreadTerminating()) return;
            Game->EntitySetThingAsAllyOfThing(Me, Game->GetHero());
            Game->EntitySetThingAsAllyOfThing(Game->GetHero(), Me);
        }
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    if (IsActiveThreadTerminating()) return;
    Game->EntitySetThingAsAllyOfThing(Me, Game->GetHero());
    Game->EntitySetThingAsAllyOfThing(Game->GetHero(), Me);
    {
        CScriptGameResourceObjectMovieBase speech_movie;
        Game->StartMovieSequence("", speech_movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
        if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
            seh_me.Speak(Game->GetHero(), "TEXT_QST_B04_BRIAR_SECONDSOUL_ONHITTWICE_10", GROUP_SELECT_NONE, false, true, false);
            while (seh_me.IsPerformingScriptTask()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
        }
    }
    Game->ActivateQuest("Q_HeroSoulsBriarMagicBarrier");
    Game->GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_HERO_SOULS_BRIAR", Game->GetActiveQuestName(), true);
    Game->KickOffQuestStartScreen(Game->GetActiveQuestName(), true, false);
    ParentClass.KickOffQuestStartScreen = true;
    Game->SetQuestCardObjective("Q_HeroSouls", "TEXT_QUEST_HERO_SOULS_OBJECTIVE_08", "NorthernWastes3", "NorthernWastes3");
    Game->DisplayQuestInfo(true);
    Game->AddQuestInfoBarHealth(Me, CRGBColour(255, 255, 0, 255), "HUD_QUEST_ICON_BRIAR_ROSE", 1.0f);
    Game->ClearThingHasInformation(Me);
    Game->MiniMapRemoveMarker(Me);
    seh_me.Release();
    ParentClass.seh_BriarRose.Release();
    Game->EntitySetAsKillable(Me, true, true);
    Game->EntitySetAsToAddToComboMultiplierWhenHit(Me, true);
    Game->EntityUnsetThingAsAllyOfThing(Me, Game->GetHero());
    Game->EntityUnsetThingAsAllyOfThing(Game->GetHero(), Me);
    Game->EntitySetAsDamageable(Me, true);
    Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
    Game->EntitySetAlwaysBlockAttacksFromThing(Me, Game->GetHero(), false);
    Game->SetIsPushableByHero(Me, true);
    {
    CTimer taunt_timer;
    taunt_timer = 15;
    long taunt_number = 20;
    bool taunted = false;
    while (Game->GetHealth(Me) > 5.0f) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (!taunted && Game->EntityGetBossPhase(Me) == 1) {
            if (IsActiveThreadTerminating()) return;
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            Game->AddLineToConversation(conversation, "TEXT_QST_B04_BRIAR_SECONDSOUL_TAUNT_10", false, Me, Game->GetHero());
            taunt_timer = 30;
            taunted = true;
        }
        if (taunt_timer.GetTime() == 0) {
            if (IsActiveThreadTerminating()) return;
            if (!Game->TextEntryExists("TEXT_QST_B04_BRIAR_SECONDSOUL_TAUNT_" + GFIntToCharString(taunt_number))) {
                if (IsActiveThreadTerminating()) return;
                taunt_number = 20;
            }
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            Game->AddLineToConversation(conversation, "TEXT_QST_B04_BRIAR_SECONDSOUL_TAUNT_" + GFIntToCharString(taunt_number), false, Me, Game->GetHero());
            taunt_timer = 35;
            taunt_number += 10;
        }
    }
    if (IsActiveThreadTerminating()) return;
    for (;;) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    }
}