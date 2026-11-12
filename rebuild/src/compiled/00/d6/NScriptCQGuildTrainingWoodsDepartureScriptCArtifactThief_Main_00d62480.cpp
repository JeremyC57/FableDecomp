// NScript::CQ_GuildTrainingWoodsDepartureScript::CArtifactThief::Main() -- the artifact thief in the departure woods
// Grade: RELOCATION_MATCH -- actual COFF operands checked against their native targets.
// Inline-budget: scoped inline_depth(0) on CIsThingConscious ctor/dtor keeps nested CScriptThing calls; retail 0x00D6249B/0x00D624BC, while marker temporaries expand. Prior hand810 plain and row-budget attempts failed.
// Leaf: ?Main@CArtifactThief@CQ_GuildTrainingWoodsDepartureScript@NScript@@EAEXXZ
// Retail: 0x00D62480, 6195 bytes, thiscall.
// Identity (proven, two sources): the Ego_r.pdb / FableWin.pdb name sets hold ?Main@CArtifactThief@CQ_GuildTrainingWoodsDepartureScript@NScript@@EAEXXZ (private virtual);
//   the address has one .rdata reference, slot 1 of the entity vtable 0x012D16DC that the landed CArtifactThief::Alloc 0x00D62360
//   stores (slot 0 ??_G 0x00D62460, 2 Init 0x00D62410, 3 GetParentScript 0x00D62400, 5 OnPredicateFail 0x00D62450).
#define FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CISTHINGALIVE_ORIGINAL_ACCESS
#define FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CPREDICATEBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CPREDICATEBASE_NOVTABLE
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/C3DVector.h"
#include "engine/EHeroAbility.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CQ_GuildTrainingWoodsDepartureScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
class CScriptGameResourceObjectMovieBase;
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();   // 0x006E7AB0
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CTimer.h"
#include "engine/CIsThingConscious.h"
#include "engine/fable_script_def_global.h"
#include "engine/fable_script_animation_defaults.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EScriptEntityMoveType { ENTITY_MOVE_WALK=0, ENTITY_MOVE_RUN=1, ENTITY_MOVE_SNEAK=2 };
extern CGameScriptInterfaceBase* pDSTGame;
inline CTimer::CTimer() { TimerIndex = pDSTGame->RegisterTimer(); }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
FABLE_FORCEINLINE CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
FABLE_FORCEINLINE long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
inline CScriptThing::~CScriptThing() { }
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
inline NScript::CIsThingConscious::CIsThingConscious(const CScriptThing& thing) : Thing(thing) { }
FABLE_FORCEINLINE NScript::CIsThingConscious::~CIsThingConscious() { }
#ifdef _MSC_VER
#pragma inline_depth()
#endif
bool FABLE_FASTCALL IsDistanceBetweenThingsUnder(const CScriptThing&, const CScriptThing&, float);   // 0x00CBE2FF
void FABLE_FASTCALL FadeOutAndKillEntity(const CScriptThing&, float, bool, bool);                    // 0x00CBE716

void NScript::CQ_GuildTrainingWoodsDepartureScript::CArtifactThief::Main()
{
    SetPredicate(CIsThingConscious(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game.StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    Game.EntitySetAsKillable(Me, false, true);
    Game.SetThingHasInformation(Me, false, true, false);
    HoldingArtifact = true;
    AlreadyTalkedTo = false;
    NotAttacked = true;
    CTimer CommentTimer;
    CommentTimer = 0;
    while (HoldingArtifact && NotAttacked) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (IsDistanceBetweenThingsUnder(Game.GetHero(), Me, 5.5f) && CommentTimer.GetTime() <= 0) {
            if (IsActiveThreadTerminating()) return;
            long Conversation = Game.AddNewConversation(Me, false, false);
            Game.AddPersonToConversation(Conversation, Game.GetHero());
            if (!AlreadyTalkedTo) {
                if (IsActiveThreadTerminating()) return;
                Game.EntitySetFacingAngleTowardsThing(Me, Game.GetHero(), false);
                seh_me.PlayAnimation("ST_WAVE_SPECIAL_02", false, false, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false);
                Game.AddLineToConversation(Conversation, "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_COMMENT_FIRST", false, Me, Game.GetHero());
            } else {
                if (IsActiveThreadTerminating()) return;
                Game.EntitySetFacingAngleTowardsThing(Me, Game.GetHero(), false);
                Game.AddLineToConversation(Conversation, "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_COMMENT_SECOND", false, Me, Game.GetHero());
            }
            CommentTimer = 10;
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return;
            NotAttacked = false;
            if (AlreadyTalkedTo) {
                if (IsActiveThreadTerminating()) return;
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game.GetHero(), "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_HIT", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                }
                Game.GiveHeroObject("OBJECT_HAND_LAMP", -1, false);
                Game.RemoveItemFromContainer(Me, "OBJECT_HAND_LAMP");
            } else {
                if (IsActiveThreadTerminating()) return;
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game.GetHero(), "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_PRE_HIT", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                }
                Game.GiveHeroObject("OBJECT_HAND_LAMP", -1, false);
                Game.RemoveItemFromContainer(Me, "OBJECT_HAND_LAMP");
            }
            Game.EntitySetAsKillable(Me, true, true);
            seh_me.MoveToPosition(Game.GetThingWithScriptName("ArtifactThiefRunMarker").GetPos(), 1.0f, ENTITY_MOVE_RUN, false, true);
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return;
            if (HoldingArtifact) {
                if (IsActiveThreadTerminating()) return;
                if (!AlreadyTalkedTo) {
                    if (IsActiveThreadTerminating()) return;
                    AlreadyTalkedTo = true;
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game.StartMovieSequence("", speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                    if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game.GetHero(), "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_CHAT", GROUP_SELECT_NONE, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return;
                        }
                        if (IsActiveThreadTerminating()) return;
                    }
                    Game.GiveHeroYesNoQuestion("TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
                    long answer = Game.MsgIsQuestionAnsweredYesOrNo();
                    while (answer < 0) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                        answer = Game.MsgIsQuestionAnsweredYesOrNo();
                    }
                    if (IsActiveThreadTerminating()) return;
                    if (answer == 1) {
                        if (IsActiveThreadTerminating()) return;
                        if (Game.GetHeroGold() < pGlobalScriptDef->GUI_LampCost) {
                            if (IsActiveThreadTerminating()) return;
                            if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                                seh_me.Speak(Game.GetHero(), "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION_NO_GOLD", GROUP_SELECT_NONE, false, true, false);
                                while (seh_me.IsPerformingScriptTask()) {
                                    Game.NewScriptFrame();
                                    if (IsActiveThreadTerminating()) return;
                                }
                                if (IsActiveThreadTerminating()) return;
                            }
                        } else {
                            if (IsActiveThreadTerminating()) return;
                            if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                                seh_me.Speak(Game.GetHero(), "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION_YES", GROUP_SELECT_NONE, false, true, false);
                                while (seh_me.IsPerformingScriptTask()) {
                                    Game.NewScriptFrame();
                                    if (IsActiveThreadTerminating()) return;
                                }
                                if (IsActiveThreadTerminating()) return;
                            }
                            Game.GiveHeroObject("OBJECT_HAND_LAMP", -1, false);
                            Game.RemoveItemFromContainer(Me, "OBJECT_HAND_LAMP");
                            Game.GiveHeroGold((long)-pGlobalScriptDef->GUI_LampCost);
                            Game.EntityGiveGold(Me, (long)pGlobalScriptDef->GUI_LampCost);
                            HoldingArtifact = false;
                            seh_me.MoveToPosition(Game.GetThingWithScriptName("ArtifactThiefRunMarker").GetPos(), 1.0f, ENTITY_MOVE_WALK, false, true);
                        }
                    } else {
                        if (IsActiveThreadTerminating()) return;
                        if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                            seh_me.Speak(Game.GetHero(), "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION_NO", GROUP_SELECT_NONE, false, true, false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return;
                            }
                            if (IsActiveThreadTerminating()) return;
                        }
                    }
                } else {
                    if (IsActiveThreadTerminating()) return;
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game.StartMovieSequence("", speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                    if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game.GetHero(), "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_RETURN", GROUP_SELECT_NONE, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return;
                        }
                        if (IsActiveThreadTerminating()) return;
                    }
                    Game.GiveHeroYesNoQuestion("TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_RETURN_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
                    long answer = Game.MsgIsQuestionAnsweredYesOrNo();
                    while (answer < 0) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                        answer = Game.MsgIsQuestionAnsweredYesOrNo();
                    }
                    if (IsActiveThreadTerminating()) return;
                    if (answer == 1) {
                        if (IsActiveThreadTerminating()) return;
                        if (Game.GetHeroGold() < pGlobalScriptDef->GUI_LampCost) {
                            if (IsActiveThreadTerminating()) return;
                            if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                                seh_me.Speak(Game.GetHero(), "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION_NO_GOLD", GROUP_SELECT_NONE, false, true, false);
                                while (seh_me.IsPerformingScriptTask()) {
                                    Game.NewScriptFrame();
                                    if (IsActiveThreadTerminating()) return;
                                }
                                if (IsActiveThreadTerminating()) return;
                            }
                        } else {
                            if (IsActiveThreadTerminating()) return;
                            if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                                seh_me.Speak(Game.GetHero(), "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION_YES", GROUP_SELECT_NONE, false, true, false);
                                while (seh_me.IsPerformingScriptTask()) {
                                    Game.NewScriptFrame();
                                    if (IsActiveThreadTerminating()) return;
                                }
                                if (IsActiveThreadTerminating()) return;
                            }
                            Game.GiveHeroObject("OBJECT_HAND_LAMP", -1, false);
                            Game.RemoveItemFromContainer(Me, "OBJECT_HAND_LAMP");
                            Game.GiveHeroGold((long)-pGlobalScriptDef->GUI_LampCost);
                            Game.EntityGiveGold(Me, (long)pGlobalScriptDef->GUI_LampCost);
                            HoldingArtifact = false;
                            seh_me.MoveToPosition(Game.GetThingWithScriptName("ArtifactThiefRunMarker").GetPos(), 1.0f, ENTITY_MOVE_WALK, false, true);
                        }
                    } else {
                        if (IsActiveThreadTerminating()) return;
                        CScriptGameResourceObjectMovieBase no_movie;
                        Game.StartMovieSequence("", no_movie);
                        CWideScreenMagicPauseEntities PauseNo(Game);
                        if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                            seh_me.Speak(Game.GetHero(), "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_RETURN_QUESTION_NO", GROUP_SELECT_NONE, false, true, false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return;
                            }
                            if (IsActiveThreadTerminating()) return;
                        }
                    }
                }
            }
        }
    }
    if (IsActiveThreadTerminating()) return;
    while (seh_me.IsPerformingScriptTask()) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return;
            if (NotAttacked) {
                if (IsActiveThreadTerminating()) return;
                long Conversation = Game.AddNewConversation(Me, false, false);
                Game.AddPersonToConversation(Conversation, Game.GetHero());
                Game.AddLineToConversation(Conversation, "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_ATTACK", false, Me, Game.GetHero());
                NotAttacked = false;
                Game.EntitySetAsKillable(Me, true, true);
                seh_me.MoveToPosition(Game.GetThingWithScriptName("ArtifactThiefRunMarker").GetPos(), 1.0f, ENTITY_MOVE_RUN, false, true);
            }
        }
    }
    if (IsActiveThreadTerminating()) return;
    FadeOutAndKillEntity(Me, 1.0f, true, true);
}
// Attempts: see STATUS.tsv / ATTEMPTS.log (best = probe/at_a3.cpp)