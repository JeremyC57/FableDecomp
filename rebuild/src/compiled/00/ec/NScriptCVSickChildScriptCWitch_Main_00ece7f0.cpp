// NScript::CV_SickChildScript::CWitch::Main() -- the witch of the sick-child quest: live while alive; take scripted control (seh_Witch); until four potion ingredients are handed over: talked to, play CS_SICK_WITCH once (objective 02), else ask TEXT_QST_B10_QUESTION_GIVE_MUSHROOM when the hero carries the ingredient (yes: take every one, NEED_MORE_CURE_10 or CS_SICK_POTION with the EndPotion + objective 03 + clear the information and leave; no: NOT_GIVEN_MORE_MUSHROOMS_10), else WAITING_FOR_CURE; hit by the hero (not by Heal Life): ONHIT_10/20/30 in turn; afterwards GOT_CURE when talked to and the same hit lines until the thread ends
// Inline-budget: header-contract inline devices and opt-ins, no flag and no pragma (ablation/, each removed in turn) -- FABLE_RETAIL_CDEFSTRING_OUTOFLINE_GETSTRING (the 2026-11-19 binding swap: retail expands the CDefString conversion and calls GetString 0x00415D70, `mov eax,esp ; push eax` at 0x00ECF317; the parked draft's OUTOFLINE_CONVERSION gave `push esp`: DIFFER(4166v4086); with neither opt-in DIFFER(4118v4086)), FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR (retail expands the pause ctors; plain inline: DIFFER(4086v4086), 605 diff lines), the row-local `inline CScriptThing::~CScriptThing() {}` (retail expands the dtor of the GetScriptThing temporaries at some sites and calls 0x004AA840 at others; without it DIFFER(4015v4086)), the inline CIsThingAlive ctor (the landed DemonDoors Mains' contract body; without it the ctor is called: DIFFER(4070v4086)) and the out-of-line `template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer()` declaration (retail calls 0x006E7AB0 inside the expanded movie dtors and the whole dtor 0x006E7B80 elsewhere; without it DIFFER(4015v4086)). No per-row flag was needed (the row matches at default flags).
// Leaf: ?Main@CWitch@CV_SickChildScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ECE7F0, 4086 bytes, thiscall, plain `ret`, frame 0xa8, no EH frame.
// Identity (two sources): Ego_r.pdb / FableWin.pdb ?Main@CWitch@CV_SickChildScript@NScript@@EAEXXZ and the Ego_d.pdb proc of the same name;
//   slot 1 of the CWitch entity vtable.
// Callees: SetPredicate 0x00F35B10 over the CIsThingAlive copy 0x004ABE90 / ~ 0x00E241B0, IsActiveThreadTerminating 0x00F35B30, ScriptedThingBase
//   IsValid 0x00CD23B9 / Release 0x00CD2770, CCharString 0x0099EBF0 / default 0x0099E4B0 / operator= 0x0099EFE0 / operator const char* 0x0099E4C0 /
//   ~ 0x0099EAE0, CDefString::GetString 0x00415D70, movie ctor 0x006E7B60 / dtor 0x006E7B80 / ~CCountedPointer<MovieBase> 0x006E7AB0, CBaseObject
//   0x0099A380 / ~ 0x0099A430, ~CBaseClass 0x0099A2E0, operator delete 0x00BFE9BC, CV_SickChildScript::PlayCutscene 0x00ECE460 (two CCharString by
//   value), pGlobalScriptDef 0x0143E90C (IngredientObject +0x738, EndPotion +0x744); CGameScriptInterfaceBase slots 7, 8, 39, 70, 114, 125, 184, 264,
//   296, 361, 370, 379, 655; ScriptedThingBase slots 12 / 13 / 26; CScriptThing slots 21 / 27 / 41 / 42.
// Source shape: the parked chunk 808/868 draft (the CSickChildsMother::Main 0x00ECD9E0 twin) with Ego_d.pdb's locals: function scope `TimesHit`,
//   `_Fail_Val`; blocks {speech_movie, PauseMyEntitiesPlease, {YNQAnswer}}, {TextToSpeak, speech_movie, PauseMyEntitiesPlease} and the post-loop
//   pair. The draft's invented `bool more` loop flag is gone (`while (IsObjectInThingsPossession(..)) {..}`, same bytes) and `text` is `TextToSpeak`.
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
#include "engine/EHeroAbility.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRING_OUTOFLINE_GETSTRING
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
    // Inferred: the `parent` reference (the landed family idiom of 0x00ECD9E0 and nine more Mains): retail forms &ParentClass.seh_Witch once
    //   (`mov eax,[esi+14h]; lea ebx,[eax+58h]`) for both the IsValid and the Release calls; through ParentClass directly it is reloaded (g2)
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
                        while (Game->IsObjectInThingsPossession(pGlobalScriptDef->IngredientObject,Game->GetHero())) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            Game->TakeObjectFromHero(pGlobalScriptDef->IngredientObject);
                            ++ThingsForPotionGot;
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
            CCharString TextToSpeak;
            switch (TimesHit) {
            case 1: TextToSpeak="TEXT_QST_B10_WITCH_ONHIT_10"; break;
            case 2: TextToSpeak="TEXT_QST_B10_WITCH_ONHIT_20"; break;
            case 3: TextToSpeak="TEXT_QST_B10_WITCH_ONHIT_30"; TimesHit=0; break;
            default: TextToSpeak="TEXT_QST_B10_WITCH_ONHIT_20"; TimesHit=0; break;
            }
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (Game->GetHealth(ParentClass.seh_Witch.GetScriptThing())>0.0f) {
                ParentClass.seh_Witch.Speak(Game->GetHero(),TextToSpeak,GROUP_SELECT_NONE,false,true,false);
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
            CCharString TextToSpeak;
            switch (TimesHit) {
            case 1: TextToSpeak="TEXT_QST_B10_WITCH_ONHIT_10"; break;
            case 2: TextToSpeak="TEXT_QST_B10_WITCH_ONHIT_20"; break;
            case 3: TextToSpeak="TEXT_QST_B10_WITCH_ONHIT_30"; TimesHit=0; break;
            default: TextToSpeak="TEXT_QST_B10_WITCH_ONHIT_20"; TimesHit=0; break;
            }
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (Game->GetHealth(ParentClass.seh_Witch.GetScriptThing())>0.0f) {
                ParentClass.seh_Witch.Speak(Game->GetHero(),TextToSpeak,GROUP_SELECT_NONE,false,true,false);
                while (ParentClass.seh_Witch.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
        }
    }
}