// NScript::CQ_HeroSoulsBriarScript::CBriarRose::Main() -- Briar Rose in the Hero Souls quest: take her over, play the soul-2 intro cut-scene once (line chosen by KilledThunder), offer information and the HUD marker, answer talks (two first lines, then sequential), turn ally when hit away from the shrine; when hit within 13 m of HS_MiddleOfShrine she speaks, starts Q_HeroSoulsBriarMagicBarrier and becomes the boss with a taunt timer until her health drops to 5
// Grade: DIFFER(3286v3260) -- 38 masked lines, nested-copy blocker: retail expands the CIsThingAlive/CIsThingConscious ctor (and dtor) but CALLS the nested CScriptThing copy 0x004ABE90 / ~CScriptThing 0x004AA840 while expanding the by-value copies in the same function; VC7.1 on the lane makes the outer inline tree all-or-nothing
// Next idea: structural, not a budget (chunk 835: retail calls the nested copy at 100% of ~500 construction sites incl. tiny ones; /d2inlT, __forceinline, definition order, pragma regions other than inline_depth, /Ob1 /Os /Ot /Oa /Gy /GX /EHsc /Zi, a two-TU /GL+/LTCG build: none reproduce it). Only a region `#pragma inline_depth(0)` round the row-local predicate ctor/dtor definitions does (diagnostic: md 14; residue = a CALL to the nested CPredicateBase() ctor); forbidden in rows -- owner question in NOTES.md
// Leaf: ?Main@CBriarRose@CQ_HeroSoulsBriarScript@NScript@@EAEXXZ
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY
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
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };
extern CGameScriptInterfaceBase* pDSTGame;               // 0x0143E8F8
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
inline NScript::CIsThingAlive::~CIsThingAlive() {}
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
    while (!IsActiveThreadTerminating())
        Game->NewScriptFrame();
    }
}
