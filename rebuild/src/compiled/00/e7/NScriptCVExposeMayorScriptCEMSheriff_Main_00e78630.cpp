// NScript::CV_ExposeMayorScript::CEM_Sheriff::Main() -- the Expose-the-Mayor sheriff: under parent scripted control play the intro cut-scene once, then until stage 5 answer the hero's talk with the stage's line (married comment once, stage 0..3 lines, at stage 4 the become-mayor question on path 1 or the path-2 line) and the hit line when attacked; after stage 5 keep answering with the path's stage-5 line
// Leaf: ?Main@CEM_Sheriff@CV_ExposeMayorScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E78630, 5045 bytes, thiscall, plain `ret`, no EH frame.
// Identity (proven, two sources): the Ego_r.pdb / FableWin.pdb name sets hold ?Main@CEM_Sheriff@CV_ExposeMayorScript@NScript@@EAEXXZ
//   (private virtual); the address has one .rdata reference, slot 1 of the entity vtable 0x012E98B4 that the landed
//   CEM_Sheriff::Alloc 0x00E76470 stores (slot 0 ??_G 0x00E76520, slot 2 Init 0x00E76540, slot 4 OnPersist 0x00E776C0, all landed).
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CV_EXPOSEMAYORSCRIPT_CEM_SHERIFF
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CV_ExposeMayorScript.h"
#include "engine/CIsThingConscious.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/ETextGroupSelectionMethod.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };

inline NScript::CIsThingConscious::CIsThingConscious(const CScriptThing& thing):Thing(thing) {}
inline NScript::CIsThingConscious::~CIsThingConscious() {}

void NScript::CV_ExposeMayorScript::CEM_Sheriff::Main()
{
    SetPredicate(CIsThingConscious(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    { CV_ExposeMayorScript& parent = ParentClass; if (parent.seh_Sheriff.IsValid()) parent.seh_Sheriff.Release(); }
    while (!Game->StartScriptingEntity(Me, ParentClass.seh_Sheriff, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    if (!IntroDone) {
        if (IsActiveThreadTerminating()) return;
        ParentClass.PlayCutscene("CS_MAYOREXPOSE_INTRO", true, "NULL");
        IntroDone = true;
    }
    while (ParentClass.StageNumber != 5) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (!CommentedOnHeroMarriedStatus && PMasterData->HeroMarriedLadyGrey) {
                if (IsActiveThreadTerminating()) return;
                CommentedOnHeroMarriedStatus = true;
                if (Game->GetHealth(ParentClass.seh_Sheriff.GetScriptThing()) > 0.0f) {
                    ParentClass.seh_Sheriff.Speak(Game->GetHero(), "TEXT_QST_B18_SHERIFF_MARRIED_COMMENT", GROUP_SELECT_NONE, false, true, false);
                    while (ParentClass.seh_Sheriff.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                }
            }
            if (ParentClass.StageNumber == 0) {
                if (IsActiveThreadTerminating()) return;
                if (Game->GetHealth(ParentClass.seh_Sheriff.GetScriptThing()) > 0.0f) {
                    ParentClass.seh_Sheriff.Speak(Game->GetHero(), "TEXT_QST_B18_SHERIFF_STAGE0", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false);
                    while (ParentClass.seh_Sheriff.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                }
            } else if (ParentClass.StageNumber == 1) {
                if (IsActiveThreadTerminating()) return;
                if (StageTwoSpoken) {
                    if (IsActiveThreadTerminating()) return;
                    if (Game->GetHealth(ParentClass.seh_Sheriff.GetScriptThing()) > 0.0f) {
                        ParentClass.seh_Sheriff.Speak(Game->GetHero(), "TEXT_QST_B18_SHERIFF_STAGE2_REPEAT", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false);
                        while (ParentClass.seh_Sheriff.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return;
                        }
                        if (IsActiveThreadTerminating()) return;
                    }
                } else {
                    if (IsActiveThreadTerminating()) return;
                    if (Game->GetHealth(ParentClass.seh_Sheriff.GetScriptThing()) > 0.0f) {
                        ParentClass.seh_Sheriff.Speak(Game->GetHero(), "TEXT_QST_B18_SHERIFF_STAGE2", GROUP_SELECT_NONE, false, true, false);
                        while (ParentClass.seh_Sheriff.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return;
                        }
                        if (IsActiveThreadTerminating()) return;
                    }
                    StageTwoSpoken = true;
                }
            } else if (ParentClass.StageNumber == 2) {
                if (IsActiveThreadTerminating()) return;
                if (Game->GetHealth(ParentClass.seh_Sheriff.GetScriptThing()) > 0.0f) {
                    ParentClass.seh_Sheriff.Speak(Game->GetHero(), "TEXT_QST_B18_SHERIFF_STAGE2_REPEAT", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false);
                    while (ParentClass.seh_Sheriff.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                }
            } else if (ParentClass.StageNumber == 3) {
                if (IsActiveThreadTerminating()) return;
                if (StageFourSpoken) {
                    if (IsActiveThreadTerminating()) return;
                    if (Game->GetHealth(ParentClass.seh_Sheriff.GetScriptThing()) > 0.0f) {
                        ParentClass.seh_Sheriff.Speak(Game->GetHero(), "TEXT_QST_B18_SHERIFF_STAGE2_REPEAT", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false);
                        while (ParentClass.seh_Sheriff.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return;
                        }
                        if (IsActiveThreadTerminating()) return;
                    }
                } else {
                    if (IsActiveThreadTerminating()) return;
                    if (Game->GetHealth(ParentClass.seh_Sheriff.GetScriptThing()) > 0.0f) {
                        ParentClass.seh_Sheriff.Speak(Game->GetHero(), "TEXT_QST_B18_SHERIFF_STAGE4", GROUP_SELECT_NONE, false, true, false);
                        while (ParentClass.seh_Sheriff.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return;
                        }
                        if (IsActiveThreadTerminating()) return;
                    }
                    StageFourSpoken = true;
                }
            } else if (ParentClass.StageNumber == 4) {
                if (IsActiveThreadTerminating()) return;
                if (ParentClass.Path == 1) {
                    if (IsActiveThreadTerminating()) return;
                    if (Game->GetHealth(ParentClass.seh_Sheriff.GetScriptThing()) > 0.0f) {
                        ParentClass.seh_Sheriff.Speak(Game->GetHero(), "TEXT_QST_B18_SHERIFF_STAGE5A", GROUP_SELECT_NONE, false, true, false);
                        while (ParentClass.seh_Sheriff.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return;
                        }
                        if (IsActiveThreadTerminating()) return;
                    }
                    Game->GiveHeroYesNoQuestion("TEXT_QST_B18_BECOME_MAYOR_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
                    long answer;
                    while ((answer = Game->MsgIsQuestionAnsweredYesOrNo()) < 0) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                    if (answer == 1) {
                        if (IsActiveThreadTerminating()) return;
                        ParentClass.SaidYesToBecomeMayor = true;
                        Game->SetThingPersistent(Me, true);
                        ParentClass.StageNumber = (STAGE_IN_PATH)5;
                    } else {
                        if (IsActiveThreadTerminating()) return;
                        Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_EXPOSE_MAYOR_OBJECTIVE_06", "", "");
                        if (Game->GetHealth(ParentClass.seh_Sheriff.GetScriptThing()) > 0.0f) {
                            ParentClass.seh_Sheriff.Speak(Game->GetHero(), "TEXT_QST_B18_SHERIFF_NOT_MAYOR", GROUP_SELECT_NONE, false, true, false);
                            while (ParentClass.seh_Sheriff.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return;
                            }
                            if (IsActiveThreadTerminating()) return;
                        }
                    }
                } else if (ParentClass.Path == 2) {
                    if (IsActiveThreadTerminating()) return;
                    if (Game->GetHealth(ParentClass.seh_Sheriff.GetScriptThing()) > 0.0f) {
                        ParentClass.seh_Sheriff.Speak(Game->GetHero(), "TEXT_QST_B18_SHERIFF_STAGE5B", GROUP_SELECT_NONE, false, true, false);
                        while (ParentClass.seh_Sheriff.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return;
                        }
                        if (IsActiveThreadTerminating()) return;
                    }
                }
            }
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return;
            Game->EntitySetThingAsAllyOfThing(Me, Game->GetHero());
            Game->EntitySetThingAsAllyOfThing(Game->GetHero(), Me);
            CScriptGameResourceObjectMovieBase hit_movie;
            Game->StartMovieSequence("", hit_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (Game->GetHealth(ParentClass.seh_Sheriff.GetScriptThing()) > 0.0f) {
                ParentClass.seh_Sheriff.Speak(Game->GetHero(), "TEXT_QST_B18_SHERIFF_ON_HIT", GROUP_SELECT_NONE, false, true, false);
                while (ParentClass.seh_Sheriff.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                }
                if (IsActiveThreadTerminating()) return;
            }
        }
    }
    if (IsActiveThreadTerminating()) return;
    while (!IsActiveThreadTerminating()) {
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (ParentClass.Path == 1) {
                if (IsActiveThreadTerminating()) return;
                if (Game->GetHealth(ParentClass.seh_Sheriff.GetScriptThing()) > 0.0f) {
                    ParentClass.seh_Sheriff.Speak(Game->GetHero(), "TEXT_QST_B18_SHERIFF_STAGE5A", GROUP_SELECT_NONE, false, true, false);
                    while (ParentClass.seh_Sheriff.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                }
            } else if (ParentClass.Path == 2) {
                if (IsActiveThreadTerminating()) return;
                if (Game->GetHealth(ParentClass.seh_Sheriff.GetScriptThing()) > 0.0f) {
                    ParentClass.seh_Sheriff.Speak(Game->GetHero(), "TEXT_QST_B18_SHERIFF_STAGE5B", GROUP_SELECT_NONE, false, true, false);
                    while (ParentClass.seh_Sheriff.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                }
            }
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return;
            Game->EntitySetThingAsAllyOfThing(Me, Game->GetHero());
            Game->EntitySetThingAsAllyOfThing(Game->GetHero(), Me);
            CScriptGameResourceObjectMovieBase hit_movie;
            Game->StartMovieSequence("", hit_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (Game->GetHealth(ParentClass.seh_Sheriff.GetScriptThing()) > 0.0f) {
                ParentClass.seh_Sheriff.Speak(Game->GetHero(), "TEXT_QST_B18_SHERIFF_ON_HIT", GROUP_SELECT_NONE, false, true, false);
                while (ParentClass.seh_Sheriff.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                }
                if (IsActiveThreadTerminating()) return;
            }
        }
        Game->NewScriptFrame();
    }
}