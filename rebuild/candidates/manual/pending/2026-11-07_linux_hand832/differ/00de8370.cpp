// NScript::CQ_RansomVictimScript::CRansommer::Main() -- the ransom kidnapper: under parent scripted control, until the victim is rescued mark him and on the first meeting play the cave cut-scene and ask the ransom question (pay 7500 gold, or 10000 once his helpers were attacked: release the victim, refuse: the attack cut-scene); then answer talks with the ask / remind / done lines and the same question, and turn bandit with the on-hit line when the hero hits him
// Grade: DIFFER(4894v4894) -- 2 instructions x2: the rotated IsPerformingScriptTask latch tests of the two block-1 not-enough-gold SAYs (0x00DE87AD, 0x00DE8A67) load ParentClass into eax + `lea ecx,[eax+0ACh]` in retail, ecx + `add ecx,0ACh` here (register colour of the latch copy only; the entry copies and all block-2 waits match)
// Next idea: chunk 832 (8 grades): an explicit rotation `if (X) do {...} while (X)` gives the same bytes (no colour change); a pointer-to-parent local, hero local, `== true`, `!= false`, `0.0f <` health, a braced in-loop T: identical or worse. The latch temp is coloured like block 2's (eax) only in retail: look at what keeps ecx busy on the exit edge of the latch in block 1 (the merged exit 0x00DE8633 / movie-scope dtors) and try a block-1 structure whose post-wait T exit shares block 2's tail
// Leaf: ?Main@CRansommer@CQ_RansomVictimScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DE8370, 4894 bytes, thiscall, plain `ret`, no EH frame.
// Identity (proven, two sources): the Ego_r.pdb / FableWin.pdb name sets hold ?Main@CRansommer@CQ_RansomVictimScript@NScript@@EAEXXZ
//   (private virtual); the address has one .rdata reference, slot 1 of the entity vtable 0x012DD1C8 that the landed
//   CRansommer::Alloc 0x00DE1BA0 stores (slot 0 ??_G 0x00DE1CA0, slot 2 Init 0x00DE1CC0, slot 3 GetParentScript 0x00DE1C40,
//   slot 5 OnPredicateFail 0x00DE1C50, all landed).
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
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };
inline NScript::CIsThingAlive::~CIsThingAlive() {}
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}

void NScript::CQ_RansomVictimScript::CRansommer::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    { CScriptGameResourceObjectScriptedThingBase& handle = ParentClass.seh_Ransommer; if (handle.IsValid()) handle.Release(); }
    while (!Game->StartScriptingEntity(Me, ParentClass.seh_Ransommer, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    if (!ParentClass.RescuedVictim) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->EntitySetInFaction(Me, "FACTION_HERO");
        Game->MiniMapAddMarker(Me, "HUD_ORB_QUEST_OPTIONAL");
        Game->SetThingHasInformation(Me, false, false, false);
    }
    if (!ParentClass.RescuedVictim) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        CScriptGameResourceObjectMovieBase speech_movie;
        Game->StartMovieSequence("", speech_movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
        if (!ParentClass.RansomHelpersAttacked) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            ParentClass.PlayCutscene("CS_RANSOM_CAVE_INTERIOR_1A", "NULL");
            Game->GiveHeroYesNoQuestion("TEXT_QST_B06_QUESTION_RANSOM_NORMAL", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
            long answer;
            while ((answer = Game->MsgIsQuestionAnsweredYesOrNo()) < 0) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (answer == 1) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game->GetHeroGold() >= 7500) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    ParentClass.PlayCutscene("CS_RANSOM_CAVE_INTERIOR_2", "TEXT_CS_B06_RANSOM_PAID_NORMAL_10");
                    ParentClass.RescuedVictim = true;
                    Game->MiniMapRemoveMarker(Me);
                    Game->ClearThingHasInformation(Me);
                    Game->GiveHeroGold(-7500);
                    Game->EntityGiveGold(Me, 7500);
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game->GetHealth(ParentClass.seh_Ransommer.GetScriptThing()) > 0.0f) {
                        ParentClass.seh_Ransommer.Speak(Game->GetHero(), "TEXT_QST_B06_RANSOM_NOT_ENOUGH_GOLD", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false);
                        while (ParentClass.seh_Ransommer.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
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
            Game->GiveHeroYesNoQuestion("TEXT_QST_B06_QUESTION_RANSOM_HIGHER", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
            long answer;
            while ((answer = Game->MsgIsQuestionAnsweredYesOrNo()) < 0) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (answer == 1) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game->GetHeroGold() >= 10000) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    ParentClass.PlayCutscene("CS_RANSOM_CAVE_INTERIOR_2", "TEXT_CS_B06_RANSOM_PAID_ATTACK_10");
                    ParentClass.RescuedVictim = true;
                    Game->MiniMapRemoveMarker(Me);
                    Game->ClearThingHasInformation(Me);
                    Game->GiveHeroGold(-10000);
                    Game->EntityGiveGold(Me, 10000);
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game->GetHealth(ParentClass.seh_Ransommer.GetScriptThing()) > 0.0f) {
                        ParentClass.seh_Ransommer.Speak(Game->GetHero(), "TEXT_QST_B06_RANSOM_NOT_ENOUGH_GOLD", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false);
                        while (ParentClass.seh_Ransommer.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
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
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (!ParentClass.RescuedVictim) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game->GetHealth(ParentClass.seh_Ransommer.GetScriptThing()) > 0.0f) {
                    ParentClass.seh_Ransommer.Speak(Game->GetHero(), "TEXT_QST_B06_RANSOM_ASK", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false);
                    while (ParentClass.seh_Ransommer.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (!ParentClass.RansomHelpersAttacked) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game->GiveHeroYesNoQuestion("TEXT_QST_B06_QUESTION_RANSOM_NORMAL", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
                    long answer;
                    while ((answer = Game->MsgIsQuestionAnsweredYesOrNo()) < 0) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (answer == 1) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game->GetHeroGold() >= 7500) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            ParentClass.RescuedVictim = true;
                            if (Game->GetHealth(ParentClass.seh_Ransommer.GetScriptThing()) > 0.0f) {
                                ParentClass.seh_Ransommer.Speak(Game->GetHero(), "TEXT_QST_B06_RANSOM_THANKS_10", GROUP_SELECT_NONE, false, true, false);
                                while (ParentClass.seh_Ransommer.IsPerformingScriptTask()) {
                                    Game->NewScriptFrame();
                                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                }
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            Game->MiniMapRemoveMarker(Me);
                            Game->ClearThingHasInformation(Me);
                            Game->GiveHeroGold(-7500);
                            Game->EntityGiveGold(Me, 7500);
                        } else {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            if (Game->GetHealth(ParentClass.seh_Ransommer.GetScriptThing()) > 0.0f) {
                                ParentClass.seh_Ransommer.Speak(Game->GetHero(), "TEXT_QST_B06_RANSOM_NOT_ENOUGH_GOLD", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false);
                                while (ParentClass.seh_Ransommer.IsPerformingScriptTask()) {
                                    Game->NewScriptFrame();
                                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                }
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                        }
                    } else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game->GetHealth(ParentClass.seh_Ransommer.GetScriptThing()) > 0.0f) {
                            ParentClass.seh_Ransommer.Speak(Game->GetHero(), "TEXT_QST_B06_RANSOM_ASK_REMIND", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false);
                            while (ParentClass.seh_Ransommer.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                    }
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game->GiveHeroYesNoQuestion("TEXT_QST_B06_QUESTION_RANSOM_HIGHER", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
                    long answer;
                    while ((answer = Game->MsgIsQuestionAnsweredYesOrNo()) < 0) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (answer == 1) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game->GetHeroGold() >= 10000) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            ParentClass.RescuedVictim = true;
                            if (Game->GetHealth(ParentClass.seh_Ransommer.GetScriptThing()) > 0.0f) {
                                ParentClass.seh_Ransommer.Speak(Game->GetHero(), "TEXT_QST_B06_RANSOM_THANKS_10", GROUP_SELECT_NONE, false, true, false);
                                while (ParentClass.seh_Ransommer.IsPerformingScriptTask()) {
                                    Game->NewScriptFrame();
                                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                }
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            Game->MiniMapRemoveMarker(Me);
                            Game->ClearThingHasInformation(Me);
                            Game->GiveHeroGold(-10000);
                            Game->EntityGiveGold(Me, 10000);
                        } else {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            if (Game->GetHealth(ParentClass.seh_Ransommer.GetScriptThing()) > 0.0f) {
                                ParentClass.seh_Ransommer.Speak(Game->GetHero(), "TEXT_QST_B06_RANSOM_NOT_ENOUGH_GOLD", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false);
                                while (ParentClass.seh_Ransommer.IsPerformingScriptTask()) {
                                    Game->NewScriptFrame();
                                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                }
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                        }
                    } else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game->GetHealth(ParentClass.seh_Ransommer.GetScriptThing()) > 0.0f) {
                            ParentClass.seh_Ransommer.Speak(Game->GetHero(), "TEXT_QST_B06_RANSOM_ASK_REMIND", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false);
                            while (ParentClass.seh_Ransommer.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                    }
                }
            } else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game->GetHealth(ParentClass.seh_Ransommer.GetScriptThing()) > 0.0f) {
                    ParentClass.seh_Ransommer.Speak(Game->GetHero(), "TEXT_QST_B06_RANSOM_DONE", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false);
                    while (ParentClass.seh_Ransommer.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->EntitySetInFaction(Me, "FACTION_BANDITS");
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            Game->AddLineToConversation(conversation, "TEXT_QST_B06_RANSOM_ON_HIT_10", false, Me, Game->GetHero());
            { CQ_RansomVictimScript& parent = ParentClass; if (parent.seh_Ransommer.IsValid()) parent.seh_Ransommer.Release(); }
            for (;;) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
        }
        Game->NewScriptFrame();
    }
}
