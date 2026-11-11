// NScript::CQ_GuildTrainingMeleeScript::CMeleeOpponent::Main() -- Whisper, the melee tutorial opponent
// Grade: RELOCATION_MATCH -- actual COFF operands checked against their native targets.
// Inline-budget: visible ordinary native timer constructor/destructor retain calls under guarded auto_inline(off); Set/Get remain forced as proven native-expanded sites.
// Leaf: ?Main@CMeleeOpponent@CQ_GuildTrainingMeleeScript@NScript@@EAEXXZ
// Retail: 0x00D56790, 6151 bytes, thiscall.
// Identity (proven, two sources): the Ego_r.pdb / FableWin.pdb name sets hold ?Main@CMeleeOpponent@CQ_GuildTrainingMeleeScript@NScript@@EAEXXZ (private virtual);
//   the address has one .rdata reference, slot 1 of the entity vtable 0x012D0328 that the landed CMeleeOpponent::Alloc 0x00D566A0
//   stores (slot 0 ??_E 0x00D56770, 2 Init 0x00D56750, 3 GetParentScript 0x00D56740).
#define FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CISTHINGALIVE_ORIGINAL_ACCESS
#define FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CPREDICATEBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CQ_GUILDTRAININGMELEESCRIPT_MELEEOPPONENT
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
#include "engine/TRetVoid.h"
#include "engine/fable_script_def_global.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
#include "engine/EScriptEntityMoveType.h"
extern CGameScriptInterfaceBase* pDSTGame;
#ifdef _MSC_VER
#pragma auto_inline(off)
#endif
CTimer::CTimer() { TimerIndex=pDSTGame->RegisterTimer(); }
CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
#ifdef _MSC_VER
#pragma auto_inline(on)
#endif
FABLE_FORCEINLINE CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
FABLE_FORCEINLINE long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }

void NScript::CQ_GuildTrainingMeleeScript::CMeleeOpponent::Main()
{
    TRetVoid _Fail_Val;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game.StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    Game.EntitySetAsKillable(Me, false, true);
    while (!ParentClass.TalkedToWhisper) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    seh_me.MoveToPosition(Game.GetThingWithScriptName("M_MeleeOpponentStand").GetPos(), 3.0f, ENTITY_MOVE_RUN, false, true);
    while (seh_me.IsPerformingScriptTask() && ParentClass.WhisperStopWalking != true) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    seh_me.ClearCommands();
    if (seh_me.IsValid()) seh_me.Release();
    ParentClass.WhisperArrived = true;
    Game.EntitySetAsKillable(Me, false, true);
    while (ParentClass.TutorialState == 1) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game.StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                seh_me.Speak(Game.GetHero(), "TEXT_QST_028_WHISPER_MEET", GROUP_SELECT_NONE, false, true, false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (seh_me.IsValid()) seh_me.Release();
        } else if (Me.MsgIsHitBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            ParentClass.EarlyHitWhisper = true;
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    while (RepeatMelee) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        while (ParentClass.TutorialState != 3) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        ParentClass.MeleeOpponentReset = false;
        Game.EntitySetCombatType(Me, "HERO_WHISPER_TUTORIAL_ATTACK_STYLE_DONT_ATTACK");
        Game.GiveThingBestEnemyTarget(Me, Game.GetHero());
        Game.EntitySetInFaction(Me, "FACTION_BANDITS");
        Me.SetFriendsWithEverythingFlag(false);
        CTimer InsultTimer;
        InsultTimer = 15;
        long BadHit = 0;
        while (ParentClass.TutorialState == 3 && ParentClass.GenericTutorialCounter < 7) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (Me.MsgIsHitByWithWeapon("SCRIPT_NAME_HERO", "OBJECT_IRON_LONGSWORD")) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                ParentClass.GenericTutorialCounter++;
                if (InsultTimer.GetTime() <= 8) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    long Conversation = Game.AddNewConversation(Me, false, false);
                    Game.AddPersonToConversation(Conversation, Game.GetHero());
                    Game.AddLineToConversation(Conversation, "TEXT_QST_028_WHISPER_MELEE_HIT_INSULT", false, Me, Game.GetHero());
                    InsultTimer = 15;
                }
            } else if (Me.MsgIsHitBy("SCRIPT_NAME_HERO")) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.ModifyThingHealth(Me, 1000.0f, false);
                if (BadHit == 0) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game.IsXbox()) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        Game.DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_SWORD_WIELD_HELP");
                        while (!Game.MsgIsGameInfoClickedPast()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    } else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        Game.DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_SWORD_WIELD_HELP_PC");
                        while (!Game.MsgIsGameInfoClickedPast()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                }
                BadHit = (BadHit + 1) % 5;
            }
            if (InsultTimer.GetTime() <= 0) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                long Conversation = Game.AddNewConversation(Me, false, false);
                Game.AddPersonToConversation(Conversation, Game.GetHero());
                Game.AddLineToConversation(Conversation, "TEXT_QST_028_WHISPER_MELEE_WAIT_INSULT", false, Me, Game.GetHero());
                InsultTimer = 15;
            }
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        while (ParentClass.TutorialState != 4) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.EntitySetCombatType(Me, "HERO_WHISPER_TUTORIAL_VS_BLOCK_ATTACK_STYLE");
        Game.GiveThingBestEnemyTarget(Me, Game.GetHero());
        Game.EntitySetInFaction(Me, "FACTION_BANDITS");
        Me.SetFriendsWithEverythingFlag(false);
        BadHit = 0;
        InsultTimer = 15;
        while (ParentClass.TutorialState == 4) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (Game.IsPlayerCreatureBlocking() && Game.GetHero().MsgIsHitBy("MeleeOpponent")) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                ParentClass.GenericTutorialCounter++;
                if (InsultTimer.GetTime() <= 8) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    long Conversation = Game.AddNewConversation(Me, false, false);
                    Game.AddPersonToConversation(Conversation, Game.GetHero());
                    Game.AddLineToConversation(Conversation, "TEXT_QST_028_WHISPER_BLOCK_HIT_INSULT", false, Me, Game.GetHero());
                    InsultTimer = 15;
                }
            } else if (Game.GetHero().MsgIsHitBy("MeleeOpponent")) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (BadHit == 0) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    CScriptThing Maze = Game.GetThingWithScriptName("TheRealGuildmaster");
                    long Conversation = Game.AddNewConversation(Maze, false, false);
                    Game.AddPersonToConversation(Conversation, Game.GetHero());
                    Game.AddLineToConversation(Conversation, "TEXT_QST_028_MAZE_BLOCK", false, Maze, Game.GetHero());
                    if (Game.IsXbox()) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        Game.DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_BLOCK_HELP");
                        while (!Game.MsgIsGameInfoClickedPast()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    } else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        Game.DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_BLOCK_HELP_PC");
                        while (!Game.MsgIsGameInfoClickedPast()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    InsultTimer = 15;
                } else if (rand() % 2 == 1) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (InsultTimer.GetTime() <= 8) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        long Conversation = Game.AddNewConversation(Me, false, false);
                        Game.AddPersonToConversation(Conversation, Game.GetHero());
                        Game.AddLineToConversation(Conversation, "TEXT_QST_028_WHISPER_BATTLE_HIT_INSULT", false, Me, Game.GetHero());
                        InsultTimer = 15;
                    }
                }
                BadHit = (BadHit + 1) % 5;
            } else if (Me.MsgIsHitBy("SCRIPT_NAME_HERO")) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (InsultTimer.GetTime() <= 8) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    long Conversation = Game.AddNewConversation(Me, false, false);
                    Game.AddPersonToConversation(Conversation, Game.GetHero());
                    Game.AddLineToConversation(Conversation, "TEXT_QST_028_WHISPER_MELEE_NOT_BLOCK", false, Me, Game.GetHero());
                    InsultTimer = 15;
                }
            }
            if (InsultTimer.GetTime() <= 0) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                long Conversation = Game.AddNewConversation(Me, false, false);
                Game.AddPersonToConversation(Conversation, Game.GetHero());
                Game.AddLineToConversation(Conversation, "TEXT_QST_028_WHISPER_BLOCK_WAIT_INSULT", false, Me, Game.GetHero());
                InsultTimer = 15;
            }
            if (Game.GetHealth(Game.GetHero()) < pGlobalScriptDef->GUI_MinHealth) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.ClearThingBestEnemyTarget(Me);
            }
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        while (ParentClass.TutorialState != 6) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.EntitySetCombatType(Me, "HERO_WHISPER_TUTORIAL_ATTACK_STYLE");
        InsultTimer = 15;
        while (ParentClass.TutorialState == 6) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (Me.MsgIsHitBy("SCRIPT_NAME_HERO")) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (InsultTimer.GetTime() <= 8) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (rand() % 2 == 1) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        long Conversation = Game.AddNewConversation(Me, false, false);
                        Game.AddPersonToConversation(Conversation, Game.GetHero());
                        Game.AddLineToConversation(Conversation, "TEXT_QST_028_WHISPER_MELEE_HIT_INSULT", false, Me, Game.GetHero());
                    } else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        CScriptThing Thunder = Game.GetThingWithScriptName("MeleeThunder");
                        long Conversation = Game.AddNewConversation(Thunder, false, false);
                        Game.AddPersonToConversation(Conversation, Game.GetHero());
                        Game.AddLineToConversation(Conversation, "TEXT_QST_028_THUNDER_MELEE_DEFEND", false, Thunder, Game.GetHero());
                    }
                    InsultTimer = 15;
                }
            } else if (Game.IsPlayerCreatureBlocking() && Game.GetHero().MsgIsHitBy("MeleeOpponent")) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (InsultTimer.GetTime() <= 8) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (rand() % 2 == 1) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        long Conversation = Game.AddNewConversation(Me, false, false);
                        Game.AddPersonToConversation(Conversation, Game.GetHero());
                        Game.AddLineToConversation(Conversation, "TEXT_QST_028_WHISPER_BLOCK_HIT_INSULT", false, Me, Game.GetHero());
                    } else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        CScriptThing Thunder = Game.GetThingWithScriptName("MeleeThunder");
                        long Conversation = Game.AddNewConversation(Thunder, false, false);
                        Game.AddPersonToConversation(Conversation, Game.GetHero());
                        Game.AddLineToConversation(Conversation, "TEXT_QST_028_THUNDER_MELEE_ATTACK", false, Thunder, Game.GetHero());
                    }
                    InsultTimer = 15;
                }
            } else if (Game.GetHero().MsgIsHitBy("MeleeOpponent")) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (InsultTimer.GetTime() <= 8) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (rand() % 2 == 1) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        long Conversation = Game.AddNewConversation(Me, false, false);
                        Game.AddPersonToConversation(Conversation, Game.GetHero());
                        Game.AddLineToConversation(Conversation, "TEXT_QST_028_WHISPER_BATTLE_HIT_INSULT", false, Me, Game.GetHero());
                    } else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        CScriptThing Thunder = Game.GetThingWithScriptName("MeleeThunder");
                        long Conversation = Game.AddNewConversation(Thunder, false, false);
                        Game.AddPersonToConversation(Conversation, Game.GetHero());
                        Game.AddLineToConversation(Conversation, "TEXT_QST_028_THUNDER_MELEE_FINISH", false, Thunder, Game.GetHero());
                    }
                    InsultTimer = 15;
                }
            }
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        while (ParentClass.TutorialState != 7) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        while (!ParentClass.MeleeRepeatKnown) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (ParentClass.MeleeRepeating) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        } else {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            RepeatMelee = false;
        }
        ParentClass.MeleeOpponentReset = true;
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}
// Attempts: chunk 810 mo_a1..a10 and chunk 828 p0, m1..m12 (STATUS.tsv / ATTEMPTS.log; best = probe/00d56790_m2.cpp)