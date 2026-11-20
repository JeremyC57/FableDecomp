// NScript::CQ_RansomVictimScript::CRansommer::Main() -- the ransom kidnapper: take him over; until the victim is rescued put him in the hero's faction with an optional-quest marker and information; at the first meeting play the cave cut-scene (1A, or 1B once his helpers were attacked) and ask the ransom question (7500 gold, or 10000 after the attack): yes with enough gold pays him, frees the victim and clears the marker, yes without enough gold gets NOT_ENOUGH_GOLD, no plays the refusal cut-scene; afterwards, whenever the hero talks to him, ask again (with ASK / THANKS / NOT_ENOUGH_GOLD / ASK_REMIND lines) or, once the victim is rescued, say DONE; when the hero hits him he joins the bandits, says ON_HIT and is released, and the thread idles until it ends
// Inline-budget: flags /d2inlS13 /d2inlT29 -- the expanded / called split of the implicit CScriptGameResourceObjectMovieBase ctor and the CWideScreenMagicPauseEntities ctor: retail CALLS the movie ctor COMDAT 0x006E7B60 at both speech scenes (0x00DE84DE, 0x00DE8B19) and EXPANDS both pause ctors (PauseAllNonScriptedEntities(true) at 0x00DE851B / 0x00DE8B56); with default flags c2 expands the second movie ctor (f=0.2367, score 29) and calls the first pause ctor (f=0.0234, score 118). T22..T29 call the movie ctor and S13..S14 expand the pause ctor; RELOCATION_MATCH for exactly S13..S14 x T22..T29 (S12 / S15 / S25 or T30 / T20 differ). The flags replace the drafts' header opt-ins FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR + FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR, which also match (ladder: flag first).
// Ablation: no flags 5054v4894; S13 alone 4990; T29 alone 4964. Source devices: none (the drafts' OUTOFLINE_DTOR, the family predicate pragma and FABLE_RETAIL_CPREDICATEBASE_NOVTABLE are unneeded here: RELOCATION_MATCH without each).
// Leaf: ?Main@CRansommer@CQ_RansomVictimScript@NScript@@EAEXXZ
// Flags: /d2inlS13 /d2inlT29
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DE8370, 4894 bytes, thiscall, plain `ret`, no EH frame.
// Identity (proven, two sources): the Ego_r.pdb / FableWin.pdb name sets hold ?Main@CRansommer@CQ_RansomVictimScript@NScript@@EAEXXZ
//   (private virtual); the address has one .rdata reference, slot 1 of the entity vtable 0x012DD1C8 that the landed
//   CRansommer::Alloc 0x00DE1BA0 stores (slot 0 ??_G 0x00DE1CA0, slot 2 Init 0x00DE1CC0, slot 3 GetParentScript 0x00DE1C40,
//   slot 5 OnPredicateFail 0x00DE1C50, all landed). Layout: engine/CQ_RansomVictimScript.h (CRansommer, ParentClass +0x14; parent
//   RescuedVictim +0x4b, RansomHelpersAttacked +0x50, seh_Ransommer +0xac).
// Source shape: Ego_d.pdb line table (lines 864..1044) and locals (_Fail_Val; speech_movie / PauseMyEntitiesPlease in both speech
//   scenes, YNQAnswer in each question block; HLStyleConversationIndex in the hit block). Each health-gated Speak + wait is one Ego_d
//   line (450 / 451 bytes), each question + answer wait one line (396). The handle releases are the same-TU siblings' spelling
//   (`{ CQ_RansomVictimScript& parent = ParentClass; ... }`, landed CVictim / CVictimsBrother / CVictimsFatherGuard::Main): retail keeps
//   &parent.seh_Ransommer in ebx / edi across IsValid and Release (0x00DE83CA, 0x00DE9652). The drafts' first release went through a
//   handle reference instead, and that alone left their two-instruction residue (ecx + `add ecx,0ACh` against retail's eax + `lea
//   ecx,[eax+0ACh]` at the block-1 NOT_ENOUGH_GOLD wait tests 0x00DE87AD / 0x00DE8A67): the hand859 draft with only that release
//   respelled is a RELOCATION_MATCH.
#include "fable_compiler.h"
#define FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CQ_RANSOMVICTIMSCRIPT_METHODS
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CQ_RansomVictimScript.h"
#include "engine/CIsThingAlive.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/ETextGroupSelectionMethod.h"
#include "engine/EHeroAbility.h"
#include "engine/TRetVoid.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
inline NScript::CIsThingAlive::~CIsThingAlive() { }

void NScript::CQ_RansomVictimScript::CRansommer::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    { CQ_RansomVictimScript& parent = ParentClass; if (parent.seh_Ransommer.IsValid()) parent.seh_Ransommer.Release(); }
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
            while ((YNQAnswer = Game.MsgIsQuestionAnsweredYesOrNo()) < 0) {
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
                        while (ParentClass.seh_Ransommer.IsPerformingScriptTask()) {
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
            while ((YNQAnswer = Game.MsgIsQuestionAnsweredYesOrNo()) < 0) {
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
                        while (ParentClass.seh_Ransommer.IsPerformingScriptTask()) {
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
    while (!IsActiveThreadTerminating()) {
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if (!ParentClass.RescuedVictim) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHealth(ParentClass.seh_Ransommer.GetScriptThing()) > 0.0f) {
                    ParentClass.seh_Ransommer.Speak(Game.GetHero(), "TEXT_QST_B06_RANSOM_ASK", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false);
                    while (ParentClass.seh_Ransommer.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (!ParentClass.RansomHelpersAttacked) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game.GiveHeroYesNoQuestion("TEXT_QST_B06_QUESTION_RANSOM_NORMAL", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
                    long YNQAnswer;
                    while ((YNQAnswer = Game.MsgIsQuestionAnsweredYesOrNo()) < 0) {
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
                                while (ParentClass.seh_Ransommer.IsPerformingScriptTask()) {
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
                                while (ParentClass.seh_Ransommer.IsPerformingScriptTask()) {
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
                            while (ParentClass.seh_Ransommer.IsPerformingScriptTask()) {
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
                    while ((YNQAnswer = Game.MsgIsQuestionAnsweredYesOrNo()) < 0) {
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
                                while (ParentClass.seh_Ransommer.IsPerformingScriptTask()) {
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
                                while (ParentClass.seh_Ransommer.IsPerformingScriptTask()) {
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
                            while (ParentClass.seh_Ransommer.IsPerformingScriptTask()) {
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
                    while (ParentClass.seh_Ransommer.IsPerformingScriptTask()) {
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
            for (;;) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
        }
        Game.NewScriptFrame();
    }
}