// NScript::CQ_TraderConflictGoodScript::CTraderToRescue::Main() -- a caged trader of the bandit-camp rescue: begs the hero (over-here / keeper-dead lines, threaten and on-hit reactions) until he talks to her, then follows him, chatters and screams while the camp region is loaded, and on reaching the teleporter thanks him and walks to it (the third trader starts the outro cut-scene)
// Inline-budget: one in-body #pragma inline_depth(0) at the thread-exit `return` after the begging loop -- retail calls every dtor there (0x00DFEA3E `jne 0x00E005D5`, the function's final pad: ~CCharString 0x0099EAE0, ~CScriptThing 0x004AA840, ~CTimer 0x00CD4470 twice, ~seh_me 0x007E74D0) while the loop-top exit just before it expands the two timers (0x00DFE9FB / 0x00DFEA0E); plain inline expands the timers at both and c2 merges the two exits into the loop's (jne 0x00DFE9E9, 16 bytes short). /d2inlT and /d2inlS move other sites first (chunk H40 sweeps).
// Leaf: ?Main@CTraderToRescue@CQ_TraderConflictGoodScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DFE0F0, 9501 bytes, thiscall, plain `ret`, no EH frame.
// Identity (proven, two sources): Ego_r.pdb and FableWin.pdb both hold ?Main@CTraderToRescue@CQ_TraderConflictGoodScript@NScript@@EAEXXZ; the address has one
//   .rdata reference, slot 1 of the entity vtable 0x012DEC68 (0x012DEC6C).
#include "fable_compiler.h"
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CQ_TRADERCONFLICTGOODSCRIPT_TRADERTORESCUE
#include <string.h>
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CQ_TraderConflictGoodScript.h"
#include "engine/CIsThingAlive.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CRGBColour.h"
#include "engine/CTimer.h"
#include "engine/fable_script_distance.h"
#include "engine/fable_script_fade.h"
#include "engine/fable_script_text.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };
enum EScriptEntityMoveType { ENTITY_MOVE_WALK=0, ENTITY_MOVE_RUN=1, ENTITY_MOVE_SNEAK=2 };
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A (ESTABLISHED_SPELLINGS: the SetDSTGame writer)
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) { }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
FABLE_FORCEINLINE CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
FABLE_FORCEINLINE long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
inline NScript::CIsThingAlive::~CIsThingAlive() {}
FABLE_FORCEINLINE bool CScriptThing::operator==(const CScriptThing& other) const { return PImp->IsEqualTo(*other.PImp); }
long FABLE_FASTCALL IsPlayerThreateningEntity(const CScriptThing&);   // 0x00CBEED2 ?IsPlayerThreateningEntity@@YIHABVCScriptThing@@@Z

void NScript::CQ_TraderConflictGoodScript::CTraderToRescue::Main()
{
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    while (!ParentClass.IntroDone) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    CTimer ThreatenTimer;
    CTimer TalkTimer;
    const CScriptThing& keeper = Game->GetNearestWithScriptName(Me, "TC_BanditHostageKeeper");
    CCharString expression;
    bool saidKeeperIsDead = false;
    // The begging loop: its only normal way out is the `break` once the hero talks to her (or opens TRADERB's cage, or beckons); the
    // thread check sits at the loop top and returns (retail 0x00DFE9E9: expression / keeper dtors called, the two timers expanded).
    // With no `return` between the loop and the rescue code the entry thread checks 0x00DFE151.. share the function's LAST epilogue
    // 0x00E00602 as retail does, and VC7.1 then folds the whole TRADERB arm into the TRADERC arm (0x00DFEDB0 `jne 0x00DFEE05`).
    for (;; Game->NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return;
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO") || (Me.GetDataString() == "TRADERB" && ParentClass.OpenedCage) || (Me.MsgExpressionPerformedTo(expression) && expression == "EXPRESSION_FOLLOW"))
            break;
        if ((TalkTimer.GetTime() == 0 || (!saidKeeperIsDead && !keeper.IsAlive())) && IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 15.0f)) {
            if (IsActiveThreadTerminating()) return;
            Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
            if (!saidKeeperIsDead && !keeper.IsAlive()) {
                if (IsActiveThreadTerminating()) return;
                long conversation = Game->AddNewConversation(Me, false, false);
                Game->AddPersonToConversation(conversation, Game->GetHero());
                Game->AddLineToConversation(conversation, "TEXT_QST_B11_" + Me.GetDataString() + "_KEEPERISDEAD", false, Me, Game->GetHero());
                saidKeeperIsDead = true;
            } else if (rand() % 5 == 0) {
                if (IsActiveThreadTerminating()) return;
                long conversation = Game->AddNewConversation(Me, false, false);
                Game->AddPersonToConversation(conversation, Game->GetHero());
                Game->AddLineToConversation(conversation, "TEXT_QST_B11_" + Me.GetDataString() + "_OVERHERE", false, Me, Game->GetHero());
            }
            TalkTimer = 10;
            ThreatenTimer = ThreatenTimer.GetTime() + 5;
        }
        if (ThreatenTimer.GetTime() == 0 && Game->GetHeroTargetedThing() == Me && Game->IsPlayerHoldingLockTargetButton()) {
            if (IsActiveThreadTerminating()) return;
            if (IsPlayerThreateningEntity(Me) && rand() % 3 == 0) {
                if (IsActiveThreadTerminating()) return;
                long conversation = Game->AddNewConversation(Me, false, false);
                Game->AddPersonToConversation(conversation, Game->GetHero());
                Game->AddLineToConversation(conversation, "TEXT_QST_B11_" + Me.GetDataString() + "_THREATENWEAPON", false, Me, Game->GetHero());
            } else {
                if (IsActiveThreadTerminating()) return;
                long conversation = Game->AddNewConversation(Me, false, false);
                Game->AddPersonToConversation(conversation, Game->GetHero());
                Game->AddLineToConversation(conversation, "TEXT_QST_B11_" + Me.GetDataString() + "_THREATEN", false, Me, Game->GetHero());
            }
            ThreatenTimer = 20;
            TalkTimer = TalkTimer.GetTime() + 5;
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return;
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            Game->AddLineToConversation(conversation, "TEXT_QST_B11_" + Me.GetDataString() + "_ONHIT", false, Me, Game->GetHero());
            ThreatenTimer = 10;
        }
    }
    if (IsActiveThreadTerminating()) {
#pragma inline_depth(0)                                  // inline control: retail calls every dtor at this exit (the shared final pad 0x00E005D5)
        return;
#pragma inline_depth()
    }
    {
        CScriptGameResourceObjectMovieBase movie;
        Game->StartMovieSequence("", movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
        if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
            seh_me.Speak(Game->GetHero(), "TEXT_QST_B11_" + Me.GetDataString() + "_INTRO", GROUP_SELECT_NONE, false, true, false);
            while (seh_me.IsPerformingScriptTask()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
        }
    }
    if (IsActiveThreadTerminating()) return;
    Game->DeactivateQuest("Q_TraderConflictGood_Extras", 0);
    Game->ActivateQuest("Q_TraderConflictGood_Extras");
    Game->SetIsPushableByHero(Me, true);
    Game->EntityFollowThing(Me, Game->GetHero(), 3.0f, true);
    Game->SetEntityAsRegionFollowing(Game->GetHero(), Me, true);
    Game->EntitySetOpinionReactionsEnabled(Me, false);
    Game->EntitySetDeedReactionsEnabled(Me, false);
    Game->EntitySetCombatEnabled(Me, false);
    Game->EntitySetInFaction(Me, "FACTION_TRADERS");
    Game->DisplayQuestInfo(true);
    if (Me.GetDataString() == "TRADERA") {
        if (IsActiveThreadTerminating()) return;
        BarIndex = Game->AddQuestInfoBarHealth(Me, CRGBColour(255, 0, 0, 255), "HUD_QUEST_ICON_TRADER", 1.0f);
    } else if (Me.GetDataString() == "TRADERB") {
        if (IsActiveThreadTerminating()) return;
        BarIndex = Game->AddQuestInfoBarHealth(Me, CRGBColour(255, 0, 0, 255), "HUD_QUEST_ICON_TRADER_HAT_02", 1.0f);
    } else if (Me.GetDataString() == "TRADERC") {
        if (IsActiveThreadTerminating()) return;
        BarIndex = Game->AddQuestInfoBarHealth(Me, CRGBColour(255, 0, 0, 255), "HUD_QUEST_ICON_TRADER_HAT_02", 1.0f);
    }
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game->IsEntityFollowingHero(Me)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    ++ParentClass.TradersFollowing;
    long talkIndex = 10;
    CCharString talkBase;
    CCharString talkLine;
    CTimer UnusedTimer;
    UnusedTimer = 20;
    CTimer ScreamTimer;
    long screamIndex = 1;
    for (; !IsActiveThreadTerminating(); Game->NewScriptFrame()) {
        while (!Game->IsRegionLoaded("BanditCampEntrance")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
            if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                if (IsActiveThreadTerminating()) return;
                talkBase = "TEXT_QST_B11_" + Me.GetDataString() + "_ONTALK_";
                talkLine = talkBase + GFIntToCharString(talkIndex);
                if (!Game->TextEntryExists(talkLine)) {
                    talkIndex = 10;
                    talkLine = talkBase + GFIntToCharString(talkIndex);
                }
                talkIndex += 10;
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                }
                if (IsActiveThreadTerminating()) return;
                CScriptGameResourceObjectMovieBase movie;
                Game->StartMovieSequence("", movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game->GetHero(), talkLine, GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                }
                seh_me.Release();
            }
            if (ThreatenTimer.GetTime() == 0 && IsPlayerThreateningEntity(Me) && Game->IsPlayerHoldingLockTargetButton()) {
                if (IsActiveThreadTerminating()) return;
                long conversation = Game->AddNewConversation(Me, false, false);
                Game->AddPersonToConversation(conversation, Game->GetHero());
                Game->AddLineToConversation(conversation, "TEXT_QST_B11_" + Me.GetDataString() + "_THREATEN", false, Me, Game->GetHero());
                ThreatenTimer = 20;
            }
            if ((Me.MsgIsHitBy("") || Me.MsgIsHitByAnySpecialAbilityFrom("")) && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO")) {
                if (IsActiveThreadTerminating()) return;
                if (Game->GetHealth(Me) > 5.0f && ScreamTimer.GetTime() == 0 && !(Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO")))) {
                    if (IsActiveThreadTerminating()) return;
                    if (Me.GetDataString() == "TRADERA") {
                        if (IsActiveThreadTerminating()) return;
                        Game->PlaySoundOnThing(Me, "SND_MM_TRADER_A_SCREAM_0" + GFIntToCharString(screamIndex));
                        if (screamIndex == 3) {
                            if (IsActiveThreadTerminating()) return;
                            screamIndex = 0;
                        }
                    } else if (Me.GetDataString() == "TRADERB") {
                        if (IsActiveThreadTerminating()) return;
                        Game->PlaySoundOnThing(Me, "SND_MM_TRADER_B_SCREAM_0" + GFIntToCharString(screamIndex));
                        if (screamIndex == 3) {
                            if (IsActiveThreadTerminating()) return;
                            screamIndex = 0;
                        }
                    } else {
                        if (IsActiveThreadTerminating()) return;
                        Game->PlaySoundOnThing(Me, "SND_MM_TRADER_C_SCREAM_0" + GFIntToCharString(screamIndex));
                        if (screamIndex == 4) {
                            if (IsActiveThreadTerminating()) return;
                            screamIndex = 0;
                        }
                    }
                    ++screamIndex;
                    ScreamTimer = 4;
                } else if (Game->GetHealth(Me) > 5.0f && (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO")))) {
                    if (IsActiveThreadTerminating()) return;
                    long conversation = Game->AddNewConversation(Me, false, false);
                    Game->AddPersonToConversation(conversation, Game->GetHero());
                    Game->AddLineToConversation(conversation, "TEXT_QST_B11_" + Me.GetDataString() + "_ONHIT", false, Me, Game->GetHero());
                }
            }
            if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
                if (IsActiveThreadTerminating()) return;
            }
            if (Me.MsgExpressionPerformedTo(expression) && expression == "EXPRESSION_WAIT") {
                if (IsActiveThreadTerminating()) return;
                --ParentClass.TradersFollowing;
            }
        }
        if (IsActiveThreadTerminating()) return;
        CScriptThing teleporter = Game->GetThingWithScriptName("TeleporterMarker");
        while (Game->IsRegionLoaded("BanditCampEntrance")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
            if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                if (IsActiveThreadTerminating()) return;
                talkBase = "TEXT_QST_B11_" + Me.GetDataString() + "_ONTALK_";
                talkLine = talkBase + GFIntToCharString(talkIndex);
                if (!Game->TextEntryExists(talkLine)) {
                    talkIndex = 10;
                    talkLine = talkBase + GFIntToCharString(talkIndex);
                }
                talkIndex += 10;
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                }
                if (IsActiveThreadTerminating()) return;
                CScriptGameResourceObjectMovieBase movie;
                Game->StartMovieSequence("", movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game->GetHero(), talkLine, GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                }
                seh_me.Release();
            }
            if (ThreatenTimer.GetTime() == 0 && IsPlayerThreateningEntity(Me) && Game->IsPlayerHoldingLockTargetButton()) {
                if (IsActiveThreadTerminating()) return;
                long conversation = Game->AddNewConversation(Me, false, false);
                Game->AddPersonToConversation(conversation, Game->GetHero());
                Game->AddLineToConversation(conversation, "TEXT_QST_B11_" + Me.GetDataString() + "_THREATEN", false, Me, Game->GetHero());
                ThreatenTimer = 20;
            }
            if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
                if (IsActiveThreadTerminating()) return;
                long conversation = Game->AddNewConversation(Me, false, false);
                Game->AddPersonToConversation(conversation, Game->GetHero());
                Game->AddLineToConversation(conversation, "TEXT_QST_B11_" + Me.GetDataString() + "_ONHIT", false, Me, Game->GetHero());
            }
            if (Me.MsgExpressionPerformedTo(expression) && expression == "EXPRESSION_WAIT") {
                if (IsActiveThreadTerminating()) return;
                --ParentClass.TradersFollowing;
            }
            if (IsDistanceBetweenThingsUnder(Me, teleporter, 20.0f)) {
                if (IsActiveThreadTerminating()) return;
                ++ParentClass.TradersReachedTeleporter;
                Game->EntityStopFollowing(Me);
                Game->SetEntityAsRegionFollowing(Game->GetHero(), Me, false);
                Game->EntitySetAsScared(Me, false);
                if (ParentClass.TradersReachedTeleporter < 3) {
                    if (IsActiveThreadTerminating()) return;
                    if (seh_me.IsValid()) seh_me.Release();
                    while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                    long conversation = Game->AddNewConversation(Me, false, false);
                    Game->AddPersonToConversation(conversation, Game->GetHero());
                    Game->AddLineToConversation(conversation, "TEXT_QST_B11_" + Me.GetDataString() + "_FREED_10", false, Me, Game->GetHero());
                    while (IsDistanceBetweenThingsOver(Me, teleporter, 2.0f) && !ParentClass.OutroStart) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                        if (!seh_me.IsPerformingScriptTask()) {
                            if (IsActiveThreadTerminating()) return;
                            seh_me.MoveToThing(teleporter, 1.0f, ENTITY_MOVE_RUN, false, true, false, true);
                        }
                    }
                    if (IsActiveThreadTerminating()) return;
                    if (ParentClass.OutroStart) {
                        if (IsActiveThreadTerminating()) return;
                        seh_me.ClearCommands();
                        seh_me.Release();
                        while (!ParentClass.OutroDone) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return;
                        }
                        if (IsActiveThreadTerminating()) return;
                        Game->RemoveThing(Me, false, true);
                        return;
                    } else {
                        if (IsActiveThreadTerminating()) return;
                        Game->RemoveQuestInfoElement(BarIndex);
                        FadeOutAndKillEntity(Me, 1.0f, true, true);
                        return;
                    }
                } else {
                    if (IsActiveThreadTerminating()) return;
                    ParentClass.OutroStart = true;
                    ParentClass.MissionSucceeded = true;
                    Game->NewScriptFrame();
                    ParentClass.PlayCutscene("CS_TRADERCON_GOOD_OUTRO");
                    ParentClass.OutroDone = true;
                    Game->RemoveThing(Me, false, true);
                    return;
                }
            }
        }
        if (IsActiveThreadTerminating()) return;
    }
}