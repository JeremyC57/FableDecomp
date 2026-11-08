// NScript::CQ_HeroSoulsThunderScript::CHeroThunder::Main() -- Thunder in the Hero Souls quest: wait for the Thunder cutscene, hold him under script control (unkillable, undamageable, marked on the mini-map) and let the hero talk to him (numbered lines chosen by Whisper's fate and Lady Grey's marriage) until the hero hits him; then play the matching on-hit line under the CAM_THUNDERHIT camera, give the quest card once, show his health bar, set the objective, free him to fight and taunt the hero every 20 s (Jack taunts once below 120 health) until his health drops to 5
// Shape (chunk 820): the closing idle loop is the chunk 817 else-wait `if (T) return; for (;;) { NSF; if (T) return; }`: VC7.1 tail-merges the
//   pre-loop test into the loop test, so the health loop exit jumps straight to retail's single bottom test (0x00D7C2A5 jmp 0x00D7C2AF); `while (!T) NSF;` duplicated the test at the head (chunk 798 park, 12 attempts)
// Leaf: ?Main@CHeroThunder@CQ_HeroSoulsThunderScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D7B510, 3524 bytes. Identity: CHeroThunder vtable slot .rdata 0x012D3664 -> ?Main@CHeroThunder@CQ_HeroSoulsThunderScript@NScript@@EAEXXZ (Ego_r.pdb + FableWin.pdb). Layout: engine/CQ_HeroSoulsThunderScript.h (new; CHeroThunder 0x1C nested).
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#include "engine/EHeroAbility.h"
#include "engine/ETextGroupSelectionMethod.h"
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CRGBColour.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "stlport/fable_map.h"
#include "engine/CQ_HeroSoulsThunderScript.h"
#include "engine/CIsThingAlive.h"
#include "engine/CTimer.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
extern "C" int __cdecl rand(void);                                       // 0x00BFEB16
extern CGameScriptInterfaceBase* pDSTGame;                               // 0x0143E8F8
CCharString FABLE_FASTCALL GFIntToCharString(long value);                // 0x0099F830
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();   // 0x006E7AB0
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) {}
inline NScript::CIsThingAlive::~CIsThingAlive() {}
FABLE_FORCEINLINE CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
FABLE_FORCEINLINE long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }

void NScript::CQ_HeroSoulsThunderScript::CHeroThunder::Main()
{
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    while (!ParentClass.PlayedThunderCS) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetThingHasInformation(Me, false, false, false);
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
    long line_number = 10;
    Game->EntitySetAlwaysBlockAttacksFromThing(Me, Game->GetHero(), true);
    Game->MiniMapAddMarker(Me, "HUD_ORB_QUEST_CORE");
    Game->EntitySetAsKillable(Me, false, false);
    Game->EntitySetAsToAddToComboMultiplierWhenHit(Me, false);
    Game->EntitySetAsDamageable(Me, false);
    Game->SetIsThingForcePushable(Me, false);
    while (!(Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO")))) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                return;
            if (ParentClass.PlayedThunderCS || ParentClass.QuestStartScreened) {
                if (IsActiveThreadTerminating())
                    return;
                CCharString text_base;
                CCharString text;
                if (PMasterData->WhisperKilledByHero && !PMasterData->HeroMarriedLadyGrey) {
                    if (IsActiveThreadTerminating())
                        return;
                    text_base = "TEXT_QST_B04_THUNDER_ONTALK_KILLEDWHISPER_NOTMARRIEDGREY_";
                } else if (!PMasterData->WhisperKilledByHero && PMasterData->HeroMarriedLadyGrey) {
                    if (IsActiveThreadTerminating())
                        return;
                    text_base = "TEXT_QST_B04_THUNDER_ONTALK_NOTKILLEDWHISPER_MARRIEDGREY_";
                } else if (!PMasterData->WhisperKilledByHero && !PMasterData->HeroMarriedLadyGrey) {
                    if (IsActiveThreadTerminating())
                        return;
                    text_base = "TEXT_QST_B04_THUNDER_ONTALK_NOTKILLEDWHISPER_NOTMARRIEDGREY_";
                } else if (PMasterData->WhisperKilledByHero && PMasterData->HeroMarriedLadyGrey) {
                    if (IsActiveThreadTerminating())
                        return;
                    text_base = "TEXT_QST_B04_THUNDER_ONTALK_KILLEDWHISPER_MARRIEDGREY_";
                }
                text = text_base + GFIntToCharString(line_number);
                if (!Game->TextEntryExists(text)) {
                    line_number = 10;
                    text = text_base + GFIntToCharString(line_number);
                }
                line_number += 10;
                CScriptGameResourceObjectMovieBase movie;
                Game->StartMovieSequence("", movie);
                CWideScreenMagicPauseEntities pause(*Game);
                if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game->GetHero(), text, GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return;
                    }
                    if (IsActiveThreadTerminating())
                        return;
                }
            } else if (IsActiveThreadTerminating()) {
                return;
            }
        }
    }
    if (IsActiveThreadTerminating())
        return;
    Game->EntitySetThingAsAllyOfThing(Me, Game->GetHero());
    Game->EntitySetThingAsAllyOfThing(Game->GetHero(), Me);
    Game->ClearThingHasInformation(Me);
    {
        CCharString text;
        if (PMasterData->WhisperKilledByHero && !PMasterData->HeroMarriedLadyGrey) {
            if (IsActiveThreadTerminating())
                return;
            text = "TEXT_QST_B04_THUNDER_KILLEDWHISPER_MARRIEDGREY_ONHIT_10";
        } else if (!PMasterData->WhisperKilledByHero && PMasterData->HeroMarriedLadyGrey) {
            if (IsActiveThreadTerminating())
                return;
            text = "TEXT_QST_B04_THUNDER_KILLEDWHISPER_NOTMARRIEDGREY_ONHIT_10";
        } else if (!PMasterData->WhisperKilledByHero && !PMasterData->HeroMarriedLadyGrey) {
            if (IsActiveThreadTerminating())
                return;
            text = "TEXT_QST_B04_THUNDER_NOTKILLEDWHISPER_NOTMARRIEDGREY_ONHIT_10";
        } else if (PMasterData->WhisperKilledByHero && PMasterData->HeroMarriedLadyGrey) {
            if (IsActiveThreadTerminating())
                return;
            text = "TEXT_QST_B04_THUNDER_NOTKILLEDWHISPER_MARRIEDGREY_ONHIT_10";
        }
        Game->FixMovieSequenceCamera(true);
        Game->CameraUseCameraPoint("CAM_THUNDERHIT", Game->GetThingWithScriptName("HeroThunder"), -1.0f, 0, -1);
        CScriptGameResourceObjectMovieBase movie;
        Game->StartMovieSequence("", movie);
        CWideScreenMagicPauseEntities pause(*Game);
        if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
            seh_me.Speak(Game->GetHero(), text, GROUP_SELECT_NONE, false, true, false);
            while (seh_me.IsPerformingScriptTask()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
        }
        Game->FadeScreenOutUntilNextCallToFadeScreenIn(0.5f, 0.5f);
        Game->Pause(0.5f);
        Game->FixMovieSequenceCamera(false);
    }
    if (!ParentClass.QuestStartScreened) {
        if (IsActiveThreadTerminating())
            return;
        Game->GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_HERO_SOULS_THUNDER", Game->GetActiveQuestName(), true);
        Game->KickOffQuestStartScreen(Game->GetActiveQuestName(), true, false);
        ParentClass.QuestStartScreened = true;
    }
    Game->FadeScreenIn();
    Game->DisplayQuestInfo(true);
    Game->AddQuestInfoBarHealth(Me, CRGBColour(255, 255, 0, 255), "HUD_QUEST_ICON_THUNDER", 1.0f);
    Game->SetQuestCardObjective("Q_HeroSouls", "TEXT_QUEST_HERO_SOULS_OBJECTIVE_05", "KnotholeGlade", "KnotholeGlade");
    Game->Pause(2.0f);
    seh_me.Release();
    Game->EntityUnsetThingAsAllyOfThing(Me, Game->GetHero());
    Game->EntityUnsetThingAsAllyOfThing(Game->GetHero(), Me);
    Game->EntitySetAsKillable(Me, true, true);
    Game->EntitySetAsToAddToComboMultiplierWhenHit(Me, true);
    Game->MiniMapRemoveMarker(Me);
    Game->EntitySetAsDamageable(Me, true);
    Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
    Game->EntitySetAlwaysBlockAttacksFromThing(Me, Game->GetHero(), false);
    {
    CTimer taunt_timer;
    taunt_timer = 15;
    bool jack_taunted = false;
    while (Game->GetHealth(Me) > 5.0f) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (taunt_timer.GetTime() == 0) {
            if (IsActiveThreadTerminating())
                return;
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            if (PMasterData->WhisperKilledByHero && rand() % 2) {
                if (IsActiveThreadTerminating())
                    return;
                Game->AddLineToConversation(conversation, "TEXT_QST_B04_THUNDER_BATTLE_TAUNT_WHISPERDEAD", false, Me, Game->GetHero());
            } else if (PMasterData->HeroMarriedLadyGrey && rand() % 2) {
                if (IsActiveThreadTerminating())
                    return;
                Game->AddLineToConversation(conversation, "TEXT_QST_B04_THUNDER_BATTLE_TAUNT_GREYMARRIED", false, Me, Game->GetHero());
            } else {
                if (IsActiveThreadTerminating())
                    return;
                Game->AddLineToConversation(conversation, "TEXT_QST_B04_THUNDER_BATTLE_TAUNT", false, Me, Game->GetHero());
            }
            taunt_timer = 20;
        }
        if (!jack_taunted && Game->GetHealth(Me) < 120.0f) {
            if (IsActiveThreadTerminating())
                return;
            long conversation = Game->AddNewConversation(Game->GetHero(), false, false);
            Game->AddLineToConversation(conversation, "TEXT_QST_B04_JACK_MSG_THUNDER_TAUNT", false, Game->GetHero(), Game->GetHero());
            jack_taunted = true;
        }
    }
    if (IsActiveThreadTerminating())
        return;
    for (;;) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    }
}