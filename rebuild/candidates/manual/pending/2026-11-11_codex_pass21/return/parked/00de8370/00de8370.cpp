// NScript::CQ_RansomVictimScript::CRansommer::Main() -- the ransom kidnapper: under parent scripted control, until the victim is rescued mark him and on the first meeting play the cave cut-scene and ask the ransom question (pay 7500 gold, or 10000 once his helpers were attacked: release the victim, refuse: the attack cut-scene); then YNQAnswer talks with the ask / remind / done lines and the same question, and turn bandit with the on-hit line when the hero hits him
// Grade: DIFFER(4894v4894) -- Exactly two task-loop latches at DE87AD and DE8A67 use ECX load + ADD ECX,AC instead of native EAX load + LEA ECX,[EAX+AC]. Same 4894 bytes and 1453 instructions; two strict / two loose hunks.
// Leaf: ?Main@CRansommer@CQ_RansomVictimScript@NScript@@EAEXXZ
// Retail: 0x00DE8370, 4894 bytes, thiscall.
// Identity (proven, two sources): the Ego_r.pdb / FableWin.pdb name sets hold ?Main@CRansommer@CQ_RansomVictimScript@NScript@@EAEXXZ
//   (private virtual); the address has one .rdata reference, slot 1 of the entity vtable 0x012DD1C8 that the landed
//   CRansommer::Alloc 0x00DE1BA0 stores (slot 0 ??_G 0x00DE1CA0, slot 2 Init 0x00DE1CC0, slot 3 GetParentScript 0x00DE1C40,
//   slot 5 OnPredicateFail 0x00DE1C50, all landed).
#define FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CISTHINGALIVE_ORIGINAL_ACCESS
#define FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CPREDICATEBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CQ_RansomVictimScript.h"
#include "engine/CIsThingAlive.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/ETextGroupSelectionMethod.h"
#include "engine/TRetVoid.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
#include "engine/EHeroAbility.h"
inline NScript::CIsThingAlive::~CIsThingAlive() {}
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}

void NScript::CQ_RansomVictimScript::CRansommer::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    { CScriptGameResourceObjectScriptedThingBase& handle = ParentClass.seh_Ransommer; if (handle.IsValid()) handle.Release(); }
    while (!Game.StartScriptingEntity(Me, ParentClass.seh_Ransommer, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    if (!ParentClass.RescuedVictim) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.EntitySetInFaction(Me, "FACTION_HERO");
        Game.MiniMapAddMarker(Me, "HUD_ORB_QUEST_OPTIONAL");
        Game.SetThingHasInformation(Me, false, false, false);
    }
    if (!ParentClass.RescuedVictim) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        CScriptGameResourceObjectMovieBase speech_movie;
        Game.StartMovieSequence("", speech_movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
        if (!ParentClass.RansomHelpersAttacked) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            ParentClass.PlayCutscene("CS_RANSOM_CAVE_INTERIOR_1A", "NULL");
            Game.GiveHeroYesNoQuestion("TEXT_QST_B06_QUESTION_RANSOM_NORMAL", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
            long YNQAnswer;
            while (!((YNQAnswer = Game.MsgIsQuestionAnsweredYesOrNo()) >= 0)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (YNQAnswer == 1) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHeroGold() >= 7500) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    ParentClass.PlayCutscene("CS_RANSOM_CAVE_INTERIOR_2", "TEXT_CS_B06_RANSOM_PAID_NORMAL_10");
                    ParentClass.RescuedVictim = true;
                    Game.MiniMapRemoveMarker(Me);
                    Game.ClearThingHasInformation(Me);
                    Game.GiveHeroGold(-7500);
                    Game.EntityGiveGold(Me, 7500);
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game.GetHealth(ParentClass.seh_Ransommer.GetScriptThing()) > 0.0f) {
                        ParentClass.seh_Ransommer.Speak(Game.GetHero(), "TEXT_QST_B06_RANSOM_NOT_ENOUGH_GOLD", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false);
                        while (!(!ParentClass.seh_Ransommer.IsPerformingScriptTask())) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                }
            } else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                ParentClass.PlayCutscene("CS_RANSOM_CAVE_INTERIOR_2", "TEXT_CS_B06_RANSOM_REFUSED_NORMAL_10");
            }
        } else {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            ParentClass.PlayCutscene("CS_RANSOM_CAVE_INTERIOR_1B", "NULL");
            Game.GiveHeroYesNoQuestion("TEXT_QST_B06_QUESTION_RANSOM_HIGHER", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
            long YNQAnswer;
            while (!((YNQAnswer = Game.MsgIsQuestionAnsweredYesOrNo()) >= 0)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (YNQAnswer == 1) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHeroGold() >= 10000) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    ParentClass.PlayCutscene("CS_RANSOM_CAVE_INTERIOR_2", "TEXT_CS_B06_RANSOM_PAID_ATTACK_10");
                    ParentClass.RescuedVictim = true;
                    Game.MiniMapRemoveMarker(Me);
                    Game.ClearThingHasInformation(Me);
                    Game.GiveHeroGold(-10000);
                    Game.EntityGiveGold(Me, 10000);
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game.GetHealth(ParentClass.seh_Ransommer.GetScriptThing()) > 0.0f) {
                        ParentClass.seh_Ransommer.Speak(Game.GetHero(), "TEXT_QST_B06_RANSOM_NOT_ENOUGH_GOLD", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false);
                        while (!(!ParentClass.seh_Ransommer.IsPerformingScriptTask())) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                }
            } else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                ParentClass.PlayCutscene("CS_RANSOM_CAVE_INTERIOR_2", "TEXT_CS_B06_RANSOM_REFUSED_ATTACK_10");
            }
        }
    }
    for (; !false; Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if (!ParentClass.RescuedVictim) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHealth(ParentClass.seh_Ransommer.GetScriptThing()) > 0.0f) {
                    ParentClass.seh_Ransommer.Speak(Game.GetHero(), "TEXT_QST_B06_RANSOM_ASK", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false);
                    while (!(!ParentClass.seh_Ransommer.IsPerformingScriptTask())) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (!ParentClass.RansomHelpersAttacked) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game.GiveHeroYesNoQuestion("TEXT_QST_B06_QUESTION_RANSOM_NORMAL", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
                    long YNQAnswer;
                    while (!((YNQAnswer = Game.MsgIsQuestionAnsweredYesOrNo()) >= 0)) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (YNQAnswer == 1) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game.GetHeroGold() >= 7500) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            ParentClass.RescuedVictim = true;
                            if (Game.GetHealth(ParentClass.seh_Ransommer.GetScriptThing()) > 0.0f) {
                                ParentClass.seh_Ransommer.Speak(Game.GetHero(), "TEXT_QST_B06_RANSOM_THANKS_10", GROUP_SELECT_NONE, false, true, false);
                                while (!(!ParentClass.seh_Ransommer.IsPerformingScriptTask())) {
                                    Game.NewScriptFrame();
                                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                }
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            Game.MiniMapRemoveMarker(Me);
                            Game.ClearThingHasInformation(Me);
                            Game.GiveHeroGold(-7500);
                            Game.EntityGiveGold(Me, 7500);
                        } else {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            if (Game.GetHealth(ParentClass.seh_Ransommer.GetScriptThing()) > 0.0f) {
                                ParentClass.seh_Ransommer.Speak(Game.GetHero(), "TEXT_QST_B06_RANSOM_NOT_ENOUGH_GOLD", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false);
                                while (!(!ParentClass.seh_Ransommer.IsPerformingScriptTask())) {
                                    Game.NewScriptFrame();
                                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                }
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                        }
                    } else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game.GetHealth(ParentClass.seh_Ransommer.GetScriptThing()) > 0.0f) {
                            ParentClass.seh_Ransommer.Speak(Game.GetHero(), "TEXT_QST_B06_RANSOM_ASK_REMIND", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false);
                            while (!(!ParentClass.seh_Ransommer.IsPerformingScriptTask())) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                    }
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game.GiveHeroYesNoQuestion("TEXT_QST_B06_QUESTION_RANSOM_HIGHER", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
                    long YNQAnswer;
                    while (!((YNQAnswer = Game.MsgIsQuestionAnsweredYesOrNo()) >= 0)) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (YNQAnswer == 1) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game.GetHeroGold() >= 10000) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            ParentClass.RescuedVictim = true;
                            if (Game.GetHealth(ParentClass.seh_Ransommer.GetScriptThing()) > 0.0f) {
                                ParentClass.seh_Ransommer.Speak(Game.GetHero(), "TEXT_QST_B06_RANSOM_THANKS_10", GROUP_SELECT_NONE, false, true, false);
                                while (!(!ParentClass.seh_Ransommer.IsPerformingScriptTask())) {
                                    Game.NewScriptFrame();
                                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                }
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            Game.MiniMapRemoveMarker(Me);
                            Game.ClearThingHasInformation(Me);
                            Game.GiveHeroGold(-10000);
                            Game.EntityGiveGold(Me, 10000);
                        } else {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            if (Game.GetHealth(ParentClass.seh_Ransommer.GetScriptThing()) > 0.0f) {
                                ParentClass.seh_Ransommer.Speak(Game.GetHero(), "TEXT_QST_B06_RANSOM_NOT_ENOUGH_GOLD", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false);
                                while (!(!ParentClass.seh_Ransommer.IsPerformingScriptTask())) {
                                    Game.NewScriptFrame();
                                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                }
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                        }
                    } else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game.GetHealth(ParentClass.seh_Ransommer.GetScriptThing()) > 0.0f) {
                            ParentClass.seh_Ransommer.Speak(Game.GetHero(), "TEXT_QST_B06_RANSOM_ASK_REMIND", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false);
                            while (!(!ParentClass.seh_Ransommer.IsPerformingScriptTask())) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                    }
                }
            } else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHealth(ParentClass.seh_Ransommer.GetScriptThing()) > 0.0f) {
                    ParentClass.seh_Ransommer.Speak(Game.GetHero(), "TEXT_QST_B06_RANSOM_DONE", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false);
                    while (!(!ParentClass.seh_Ransommer.IsPerformingScriptTask())) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.EntitySetInFaction(Me, "FACTION_BANDITS");
            long HLStyleConversationIndex = Game.AddNewConversation(Me, false, false);
            Game.AddPersonToConversation(HLStyleConversationIndex, Game.GetHero());
            Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_B06_RANSOM_ON_HIT_10", false, Me, Game.GetHero());
            { CQ_RansomVictimScript& parent = ParentClass; if (parent.seh_Ransommer.IsValid()) parent.seh_Ransommer.Release(); }
            while (true) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}
