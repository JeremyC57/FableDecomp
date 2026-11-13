// NScript::CV_SickChildScript::CWitch::Main() -- the witch of the sick-child quest: live while alive; take scripted control (seh_Witch); until four potion ingredients are handed over: talked to, play CS_SICK_WITCH once (objective 02), else ask TEXT_QST_B10_QUESTION_GIVE_MUSHROOM when the hero carries the ingredient (yes: take every one, NEED_MORE_CURE_10 or CS_SICK_POTION + objective 03 + clear information and leave the loop; no: NOT_GIVEN_MORE_MUSHROOMS_10) or say WAITING_FOR_CURE; hit by the hero (except a heal-life spell) say ONHIT_10/20/30 in turn; afterwards: talked to say GOT_CURE, hit say the ONHIT lines, until the thread ends
// Grade: DIFFER(4086v4086) -- md=4: 0x00ECF317 retail `mov eax,esp ; push eax` for the hidden return slot of CDefString::operator CCharString (0x00415D70) in `ParentClass.PlayCutscene("CS_SICK_POTION", pGlobalScriptDef->EndPotion)`, ours `push esp`; chunk 868: the exe scan of all 90 calls of 0x00415D70 finds this the only top-of-stack hidden-pointer site (no retail sibling shows which spelling folds), the QFE 4035 toolset also folds, the rotation probe (probe/00ece7f0.rot.out) shows retail's allocation everywhere else; 40 earlier spellings (casts, temporaries, operator*/->, named refs, GetString, flags)
// Next idea: the fold of `lea eax,[esp] ; push eax` into `push esp` is the quantity: a spelling that forms the conversion's `this` (`mov ecx,[pGlobalScriptDef] ; add ecx,744h`) between the lea and the push (the def pointer through a local before the call) might block it
// Leaf: ?Main@CWitch@CV_SickChildScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ECE7F0, 4086 bytes, thiscall, plain `ret`, frame 0xa8.
// Identity (proven): Ego_r.pdb ?Main@CWitch@CV_SickChildScript@NScript@@EAEXXZ; slot 1 of the CWitch entity vtable.
// Shape: the CSickChildsMother::Main twin (0x00ECD9E0) on the same headers; the movie ctor / dtor stay plain inline (retail expands some sites
//   and calls 0x006E7B60 / 0x006E7B80 at others).
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#include "engine/C3DVector.h"
#include "engine/CV_SickChildScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CCountedPointer.h"
class CScriptGameResourceObjectMovieBase;
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/TRetVoid.h"
#include "engine/CIsThingAlive.h"
#include "engine/ETextGroupSelectionMethod.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRING_OUTOFLINE_CONVERSION
#define FABLE_RETAIL_CDEFSTRINGTABLE
#include "engine/fable_script_def_global.h"
inline CScriptThing::~CScriptThing() {}
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
void NScript::CV_SickChildScript::CWitch::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    {
        CV_SickChildScript& parent = ParentClass;
        if (parent.seh_Witch.IsValid()) parent.seh_Witch.Release();
    }
    while (!Game->StartScriptingEntity(Me,ParentClass.seh_Witch,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    long TimesHit=0;
    while (ThingsForPotionGot<4) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (!IntroDone) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                ParentClass.PlayCutscene("CS_SICK_WITCH","NULL");
                Game->SetQuestCardObjective(Game->GetActiveQuestName(),"TEXT_QUEST_SICK_CHILD_OBJECTIVE_02","","");
                IntroDone=true;
            } else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game->IsObjectInThingsPossession(pGlobalScriptDef->IngredientObject,Game->GetHero()) && ThingsForPotionGot<4) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game->GiveHeroYesNoQuestion("TEXT_QST_B10_QUESTION_GIVE_MUSHROOM","TEXT_OBJECT_HERO_ANSWER_YES","TEXT_OBJECT_HERO_ANSWER_NO","",true);
                    long YNQAnswer;
                    while ((YNQAnswer=Game->MsgIsQuestionAnsweredYesOrNo())<0) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (YNQAnswer==1) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        bool more=true;
                        while (more) {
                            if (Game->IsObjectInThingsPossession(pGlobalScriptDef->IngredientObject,Game->GetHero())) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                Game->TakeObjectFromHero(pGlobalScriptDef->IngredientObject);
                                ++ThingsForPotionGot;
                            } else more=false;
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (ThingsForPotionGot<4) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            if (Game->GetHealth(ParentClass.seh_Witch.GetScriptThing())>0.0f) {
                                ParentClass.seh_Witch.Speak(Game->GetHero(),"TEXT_QST_B10_WITCH_NEED_MORE_CURE_10",GROUP_SELECT_NONE,false,true,false);
                                while (ParentClass.seh_Witch.IsPerformingScriptTask()) {
                                    Game->NewScriptFrame();
                                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                }
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                        } else {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            ParentClass.PlayCutscene("CS_SICK_POTION",pGlobalScriptDef->EndPotion);
                            Game->SetQuestCardObjective(Game->GetActiveQuestName(),"TEXT_QUEST_SICK_CHILD_OBJECTIVE_03","","");
                            Game->ClearThingHasInformation(Me);
                            break;
                        }
                    } else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game->GetHealth(ParentClass.seh_Witch.GetScriptThing())>0.0f) {
                            ParentClass.seh_Witch.Speak(Game->GetHero(),"TEXT_QST_B10_WITCH_NOT_GIVEN_MORE_MUSHROOMS_10",GROUP_SELECT_NONE,false,true,false);
                            while (ParentClass.seh_Witch.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                    }
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game->GetHealth(ParentClass.seh_Witch.GetScriptThing())>0.0f) {
                        ParentClass.seh_Witch.Speak(Game->GetHero(),"TEXT_QST_B10_WITCH_WAITING_FOR_CURE",GROUP_SELECT_NONE,false,true,false);
                        while (ParentClass.seh_Witch.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                }
            }
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            ++TimesHit;
            CCharString text;
            switch (TimesHit) {
            case 1: text="TEXT_QST_B10_WITCH_ONHIT_10"; break;
            case 2: text="TEXT_QST_B10_WITCH_ONHIT_20"; break;
            case 3: text="TEXT_QST_B10_WITCH_ONHIT_30"; TimesHit=0; break;
            default: text="TEXT_QST_B10_WITCH_ONHIT_20"; TimesHit=0; break;
            }
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (Game->GetHealth(ParentClass.seh_Witch.GetScriptThing())>0.0f) {
                ParentClass.seh_Witch.Speak(Game->GetHero(),text,GROUP_SELECT_NONE,false,true,false);
                while (ParentClass.seh_Witch.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    for (; !IsActiveThreadTerminating(); Game->NewScriptFrame()) {
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (Game->GetHealth(ParentClass.seh_Witch.GetScriptThing())>0.0f) {
                ParentClass.seh_Witch.Speak(Game->GetHero(),"TEXT_QST_B10_WITCH_GOT_CURE",GROUP_SELECT_RANDOM_NO_REPEAT,false,true,false);
                while (ParentClass.seh_Witch.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            ++TimesHit;
            CCharString text;
            switch (TimesHit) {
            case 1: text="TEXT_QST_B10_WITCH_ONHIT_10"; break;
            case 2: text="TEXT_QST_B10_WITCH_ONHIT_20"; break;
            case 3: text="TEXT_QST_B10_WITCH_ONHIT_30"; TimesHit=0; break;
            default: text="TEXT_QST_B10_WITCH_ONHIT_20"; TimesHit=0; break;
            }
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (Game->GetHealth(ParentClass.seh_Witch.GetScriptThing())>0.0f) {
                ParentClass.seh_Witch.Speak(Game->GetHero(),text,GROUP_SELECT_NONE,false,true,false);
                while (ParentClass.seh_Witch.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
        }
    }
}
