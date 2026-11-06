// NScript::CQ_GuildTrainingMeleeScript::CMeleeOpponent::Main() -- Whisper, the melee tutorial opponent
// Grade: DIFFER(6134v6151) -- SDIFF 27 (the 836 diff lines are the 17-byte shift): the stage-3 GenericTutorialCounter++ is load / inc / store in retail with the InsultTimer index load scheduled between (ours `inc [mem]` as in retail stage 4); stage-6 register colouring (retail keeps InsultTimer.TimerIndex in ebp across the stage-6 loop, loads ebx = 6 before the thread check, and the whisper conversation strings go through ecx); 3 loop-head jmp-over-pad differences that follow the shift
// Next idea: the stage-3 increment spelling VC7.1 does not fuse into inc [mem] (x = x + 1, += 1, ++x, a named local and a CTimer::operator<= test were identical); the pads should then follow
// Leaf: ?Main@CMeleeOpponent@CQ_GuildTrainingMeleeScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D56790, 6151 bytes, thiscall, plain `ret`, no EH frame.
// Identity (proven, two sources): the Ego_r.pdb / FableWin.pdb name sets hold ?Main@CMeleeOpponent@CQ_GuildTrainingMeleeScript@NScript@@EAEXXZ (private virtual);
//   the address has one .rdata reference, slot 1 of the entity vtable 0x012D0328 that the landed CMeleeOpponent::Alloc 0x00D566A0
//   stores (slot 0 ??_E 0x00D56770, 2 Init 0x00D56750, 3 GetParentScript 0x00D56740).
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_FORCEINLINE_CTOR
#include <stdlib.h>
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/C3DVector.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CQ_GuildTrainingMeleeScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
class CScriptGameResourceObjectMovieBase;
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();   // 0x006E7AB0
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CTimer.h"
#include "engine/fable_script_def_global.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EScriptEntityMoveType { ENTITY_MOVE_WALK=0, ENTITY_MOVE_RUN=1, ENTITY_MOVE_SNEAK=2 };
extern CGameScriptInterfaceBase* pDSTGame;
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }

void NScript::CQ_GuildTrainingMeleeScript::CMeleeOpponent::Main()
{
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    Game->EntitySetAsKillable(Me, false, true);
    while (!ParentClass->TalkedToWhisper) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    seh_me.MoveToPosition(Game->GetThingWithScriptName("M_MeleeOpponentStand").GetPos(), 3.0f, ENTITY_MOVE_RUN, false, true);
    while (seh_me.IsPerformingScriptTask() && ParentClass->WhisperStopWalking != true) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    seh_me.ClearCommands();
    if (seh_me.IsValid()) seh_me.Release();
    ParentClass->WhisperArrived = true;
    Game->EntitySetAsKillable(Me, false, true);
    while (ParentClass->TutorialState == 1) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                seh_me.Speak(Game->GetHero(), "TEXT_QST_028_WHISPER_MEET", GROUP_SELECT_NONE, false, true, false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                }
                if (IsActiveThreadTerminating()) return;
            }
            if (seh_me.IsValid()) seh_me.Release();
        } else if (Me.MsgIsHitBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return;
            ParentClass->EarlyHitWhisper = true;
        }
    }
    if (IsActiveThreadTerminating()) return;
    while (RepeatMelee) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        while (ParentClass->TutorialState != 3) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        ParentClass->MeleeOpponentReset = false;
        Game->EntitySetCombatType(Me, "HERO_WHISPER_TUTORIAL_ATTACK_STYLE_DONT_ATTACK");
        Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
        Game->EntitySetInFaction(Me, "FACTION_BANDITS");
        Me.SetFriendsWithEverythingFlag(false);
        CTimer InsultTimer;
        InsultTimer = 15;
        long HitCount = 0;
        while (ParentClass->TutorialState == 3 && ParentClass->GenericTutorialCounter < 7) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
            if (Me.MsgIsHitByWithWeapon("SCRIPT_NAME_HERO", "OBJECT_IRON_LONGSWORD")) {
                if (IsActiveThreadTerminating()) return;
                ParentClass->GenericTutorialCounter = ParentClass->GenericTutorialCounter + 1;
                if (InsultTimer.GetTime() <= 8) {
                    if (IsActiveThreadTerminating()) return;
                    long Conversation = Game->AddNewConversation(Me, false, false);
                    Game->AddPersonToConversation(Conversation, Game->GetHero());
                    Game->AddLineToConversation(Conversation, "TEXT_QST_028_WHISPER_MELEE_HIT_INSULT", false, Me, Game->GetHero());
                    InsultTimer = 15;
                }
            } else if (Me.MsgIsHitBy("SCRIPT_NAME_HERO")) {
                if (IsActiveThreadTerminating()) return;
                Game->ModifyThingHealth(Me, 1000.0f, false);
                if (HitCount == 0) {
                    if (IsActiveThreadTerminating()) return;
                    if (Game->IsXbox()) {
                        if (IsActiveThreadTerminating()) return;
                        Game->DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_SWORD_WIELD_HELP");
                        while (!Game->MsgIsGameInfoClickedPast()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return;
                        }
                    } else {
                        if (IsActiveThreadTerminating()) return;
                        Game->DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_SWORD_WIELD_HELP_PC");
                        while (!Game->MsgIsGameInfoClickedPast()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return;
                        }
                    }
                    if (IsActiveThreadTerminating()) return;
                }
                HitCount = (HitCount + 1) % 5;
            }
            if (InsultTimer.GetTime() <= 0) {
                if (IsActiveThreadTerminating()) return;
                long Conversation = Game->AddNewConversation(Me, false, false);
                Game->AddPersonToConversation(Conversation, Game->GetHero());
                Game->AddLineToConversation(Conversation, "TEXT_QST_028_WHISPER_MELEE_WAIT_INSULT", false, Me, Game->GetHero());
                InsultTimer = 15;
            }
        }
        if (IsActiveThreadTerminating()) return;
        while (ParentClass->TutorialState != 4) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        Game->EntitySetCombatType(Me, "HERO_WHISPER_TUTORIAL_VS_BLOCK_ATTACK_STYLE");
        Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
        Game->EntitySetInFaction(Me, "FACTION_BANDITS");
        Me.SetFriendsWithEverythingFlag(false);
        HitCount = 0;
        InsultTimer = 15;
        while (ParentClass->TutorialState == 4) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
            if (Game->IsPlayerCreatureBlocking() && Game->GetHero().MsgIsHitBy("MeleeOpponent")) {
                if (IsActiveThreadTerminating()) return;
                ParentClass->GenericTutorialCounter++;
                if (InsultTimer.GetTime() <= 8) {
                    if (IsActiveThreadTerminating()) return;
                    long Conversation = Game->AddNewConversation(Me, false, false);
                    Game->AddPersonToConversation(Conversation, Game->GetHero());
                    Game->AddLineToConversation(Conversation, "TEXT_QST_028_WHISPER_BLOCK_HIT_INSULT", false, Me, Game->GetHero());
                    InsultTimer = 15;
                }
            } else if (Game->GetHero().MsgIsHitBy("MeleeOpponent")) {
                if (IsActiveThreadTerminating()) return;
                if (HitCount == 0) {
                    if (IsActiveThreadTerminating()) return;
                    CScriptThing Guildmaster = Game->GetThingWithScriptName("TheRealGuildmaster");
                    long Conversation = Game->AddNewConversation(Guildmaster, false, false);
                    Game->AddPersonToConversation(Conversation, Game->GetHero());
                    Game->AddLineToConversation(Conversation, "TEXT_QST_028_MAZE_BLOCK", false, Guildmaster, Game->GetHero());
                    if (Game->IsXbox()) {
                        if (IsActiveThreadTerminating()) return;
                        Game->DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_BLOCK_HELP");
                        while (!Game->MsgIsGameInfoClickedPast()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return;
                        }
                    } else {
                        if (IsActiveThreadTerminating()) return;
                        Game->DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_BLOCK_HELP_PC");
                        while (!Game->MsgIsGameInfoClickedPast()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return;
                        }
                    }
                    if (IsActiveThreadTerminating()) return;
                    InsultTimer = 15;
                } else if (rand() % 2 == 1) {
                    if (IsActiveThreadTerminating()) return;
                    if (InsultTimer.GetTime() <= 8) {
                        if (IsActiveThreadTerminating()) return;
                        long Conversation = Game->AddNewConversation(Me, false, false);
                        Game->AddPersonToConversation(Conversation, Game->GetHero());
                        Game->AddLineToConversation(Conversation, "TEXT_QST_028_WHISPER_BATTLE_HIT_INSULT", false, Me, Game->GetHero());
                        InsultTimer = 15;
                    }
                }
                HitCount = (HitCount + 1) % 5;
            } else if (Me.MsgIsHitBy("SCRIPT_NAME_HERO")) {
                if (IsActiveThreadTerminating()) return;
                if (InsultTimer.GetTime() <= 8) {
                    if (IsActiveThreadTerminating()) return;
                    long Conversation = Game->AddNewConversation(Me, false, false);
                    Game->AddPersonToConversation(Conversation, Game->GetHero());
                    Game->AddLineToConversation(Conversation, "TEXT_QST_028_WHISPER_MELEE_NOT_BLOCK", false, Me, Game->GetHero());
                    InsultTimer = 15;
                }
            }
            if (InsultTimer.GetTime() <= 0) {
                if (IsActiveThreadTerminating()) return;
                long Conversation = Game->AddNewConversation(Me, false, false);
                Game->AddPersonToConversation(Conversation, Game->GetHero());
                Game->AddLineToConversation(Conversation, "TEXT_QST_028_WHISPER_BLOCK_WAIT_INSULT", false, Me, Game->GetHero());
                InsultTimer = 15;
            }
            if (Game->GetHealth(Game->GetHero()) < pGlobalScriptDef->GUI_MinHealth) {
                if (IsActiveThreadTerminating()) return;
                Game->ClearThingBestEnemyTarget(Me);
            }
        }
        if (IsActiveThreadTerminating()) return;
        while (ParentClass->TutorialState != 6) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        Game->EntitySetCombatType(Me, "HERO_WHISPER_TUTORIAL_ATTACK_STYLE");
        InsultTimer = 15;
        while (ParentClass->TutorialState == 6) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
            if (Me.MsgIsHitBy("SCRIPT_NAME_HERO")) {
                if (IsActiveThreadTerminating()) return;
                if (InsultTimer.GetTime() <= 8) {
                    if (IsActiveThreadTerminating()) return;
                    if (rand() % 2 == 1) {
                        if (IsActiveThreadTerminating()) return;
                        long Conversation = Game->AddNewConversation(Me, false, false);
                        Game->AddPersonToConversation(Conversation, Game->GetHero());
                        Game->AddLineToConversation(Conversation, "TEXT_QST_028_WHISPER_MELEE_HIT_INSULT", false, Me, Game->GetHero());
                    } else {
                        if (IsActiveThreadTerminating()) return;
                        CScriptThing Thunder = Game->GetThingWithScriptName("MeleeThunder");
                        long Conversation = Game->AddNewConversation(Thunder, false, false);
                        Game->AddPersonToConversation(Conversation, Game->GetHero());
                        Game->AddLineToConversation(Conversation, "TEXT_QST_028_THUNDER_MELEE_DEFEND", false, Thunder, Game->GetHero());
                    }
                    InsultTimer = 15;
                }
            } else if (Game->IsPlayerCreatureBlocking() && Game->GetHero().MsgIsHitBy("MeleeOpponent")) {
                if (IsActiveThreadTerminating()) return;
                if (InsultTimer.GetTime() <= 8) {
                    if (IsActiveThreadTerminating()) return;
                    if (rand() % 2 == 1) {
                        if (IsActiveThreadTerminating()) return;
                        long Conversation = Game->AddNewConversation(Me, false, false);
                        Game->AddPersonToConversation(Conversation, Game->GetHero());
                        Game->AddLineToConversation(Conversation, "TEXT_QST_028_WHISPER_BLOCK_HIT_INSULT", false, Me, Game->GetHero());
                    } else {
                        if (IsActiveThreadTerminating()) return;
                        CScriptThing Thunder = Game->GetThingWithScriptName("MeleeThunder");
                        long Conversation = Game->AddNewConversation(Thunder, false, false);
                        Game->AddPersonToConversation(Conversation, Game->GetHero());
                        Game->AddLineToConversation(Conversation, "TEXT_QST_028_THUNDER_MELEE_ATTACK", false, Thunder, Game->GetHero());
                    }
                    InsultTimer = 15;
                }
            } else if (Game->GetHero().MsgIsHitBy("MeleeOpponent")) {
                if (IsActiveThreadTerminating()) return;
                if (InsultTimer.GetTime() <= 8) {
                    if (IsActiveThreadTerminating()) return;
                    if (rand() % 2 == 1) {
                        if (IsActiveThreadTerminating()) return;
                        long Conversation = Game->AddNewConversation(Me, false, false);
                        Game->AddPersonToConversation(Conversation, Game->GetHero());
                        Game->AddLineToConversation(Conversation, "TEXT_QST_028_WHISPER_BATTLE_HIT_INSULT", false, Me, Game->GetHero());
                    } else {
                        if (IsActiveThreadTerminating()) return;
                        CScriptThing Thunder = Game->GetThingWithScriptName("MeleeThunder");
                        long Conversation = Game->AddNewConversation(Thunder, false, false);
                        Game->AddPersonToConversation(Conversation, Game->GetHero());
                        Game->AddLineToConversation(Conversation, "TEXT_QST_028_THUNDER_MELEE_FINISH", false, Thunder, Game->GetHero());
                    }
                    InsultTimer = 15;
                }
            }
        }
        if (IsActiveThreadTerminating()) return;
        while (ParentClass->TutorialState != 7) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        while (!ParentClass->MeleeRepeatKnown) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        if (ParentClass->MeleeRepeating) {
            if (IsActiveThreadTerminating()) return;
        } else {
            if (IsActiveThreadTerminating()) return;
            RepeatMelee = false;
        }
        ParentClass->MeleeOpponentReset = true;
    }
    if (IsActiveThreadTerminating()) return;
}
// Attempts: see STATUS.tsv / ATTEMPTS.log (best = probe/mo_a2.cpp)
