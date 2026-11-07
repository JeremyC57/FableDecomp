// NScript::CV_GhostGrannyNecklaceScript::CGhostGrannySon::Main() -- the ghost granny's son at Orchard Farm: when talked to or shown the necklace, thanks the hero or sends him to his mother once it is returned; if the hero carries the necklace asks for it and plays the CS_GHOSTGRANNY_OUTRO hand-over (necklace taken, gold / morality / experience reward); otherwise plays the intro cut scene the first time (or the "oh mother" line), then offers the quest card; when hit to near death runs away and is removed
// Grade: DIFFER(6038v5996) -- one inline-threshold split of the implicit seh_me (STB) dtor: retail calls 0x007E74D0 at the talk-block T pad (0x00E8F176) and expands the loop-head exit; VC7.1 expands both here (59 masked lines); grades RELOCATION_MATCH only under the diagnostic /d2inlT72 (not a landing flag); chunk 826: a row-local cost-only ~CCountedPointer<STB> spec (ReleaseData; Data = 0; Info = 0) is a no-op (the inliner sees the optimised callee), and the while (!T) loop form is far worse (md 1028)
// Next idea: the lane-wide STB-dtor inline cost (same split as 0x00DAC760), checked over the landed STB rows; known residue class (per-exit inline split): park
// Leaf: ?Main@CGhostGrannySon@CV_GhostGrannyNecklaceScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E8EF80, thiscall, no EH frame.
// Identity (proven, two sources): the address appears once in .rdata, at slot 1 of the entity vtable 0x012EBC34 stored by
//   CGhostGrannySon::Alloc 0x00E8CA60 (slot 0 the landed scalar deleting dtor 0x00E8CB20, slot 2 the landed Init 0x00E8CB10, slot 3 the
//   landed GetParentScript 0x00E8CB00); Ego_r.pdb and FableWin.pdb both hold ?Main@CGhostGrannySon@CV_GhostGrannyNecklaceScript@NScript@@EAEXXZ.
// Callees: SetPredicate 0x00F35B10 with the CIsThingAlive temporary (vtable 0x012C32F0, ctor inline, dtor 0x00E241B0),
//   IsActiveThreadTerminating 0x00F35B30, CScriptThing copy 0x004ABE90 / ~ 0x004AA840, the scripted-thing handle (CBaseObject
//   0x0099A380, ctor 0x007E72A0, IsValid 0x00CD23B9, Release 0x00CD2770, Speak 0x007E7390, IsPerformingScriptTask 0x007E7450,
//   GetScriptThing 0x007E7490, operator= 0x008ABD10, ~ 0x007E74D0), MovieBase ctor 0x006E7B60 / dtor 0x006E7B80, cast map ctor
//   0x00CDBF70 / dtor 0x00CDBFB0 / operator[] 0x00CD3D2E, RunCutsceneMacro 0x00CBFB7D, CCharString 0x0099E4B0 / 0x0099EBF0 / ~
//   0x0099EAE0, CBasicString<char>::Compare 0x00411570; interface slots through Game.
// Data: ParentClass (+0x14) WifeNeededForCutscene +0x48, HeroKnowsAboutNecklace +0x49, NecklaceReturned +0x4A, GhostToRest +0x4E,
//   QuestCardGiven +0x4F; PMasterData (+0x18) OrchardFarmRaidLastCompleted +0x58; own TalkCounter +0x1C, ShownItem +0x20;
//   pGlobalScriptDef GGN_ReturnNecklaceReward +0x658, GGN_ReturnNecklaceMoralityGain +0x664, GGN_ReturnNecklaceExperienceGain +0x668.
// Shape: the landed sibling CGhostGrannyDaughterInLaw::Main 0x00E8BEF0 and the cast-map cut scene of CGhostGrannyNecklace::Main
//   0x00E8DFA0; the main loop is `for (;; NewScriptFrame())` with the T-check at the top of the body (in a do-while(0); same code).
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#include <string.h>
#include "engine/C3DVector.h"
#include "engine/CRGBColour.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CV_GhostGrannyNecklaceScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/fable_script_cutscene.h"
#include "engine/CScriptDef.h"
#include "engine/CDefPointer.h"
#include "engine/TRetVoid.h"
#include "engine/CIsThingAlive.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };
extern CDefPointer<const CScriptDef> pGlobalScriptDef;
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::map();
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::~map();
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
// The body of CBasicString<char>::Compare 0x00411570 (the landed 0x00411570 loop; chunk 807 lesson): same-TU and not inlined, as in
// retail, so VC7.1 knows the call leaves the string alone.
template<> inline __declspec(noinline) long __fastcall CBasicString<char>::Compare(const char* a, const char* b)
{
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        char cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}

void NScript::CV_GhostGrannyNecklaceScript::CGhostGrannySon::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    Game->EntitySetAsKillable(Me,false,true);
    Game->EntitySetAllStategroupsEnabled(Me,false);
    Game->EntitySetOpinionReactionsEnabled(Me,true);
    Game->EntitySetDeedReactionsEnabled(Me,true);
    Game->EntitySetOpinionReactionMask(Me,"OPINION_REACTION_MASK_EX_SCRIPT_ACQUAINTANCE");
    CCharString PresentedItem;
    if (!ParentClass.NecklaceReturned) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->SetThingHasInformation(Me,false,false,false);
    }
    Game->EntitySetFacingAngleTowardsThing(Me,Game->GetHero(),true);
    for (;; Game->NewScriptFrame()) {
        do { if (IsActiveThreadTerminating()) return _Fail_Val.Return(); } while (0);
        if (Me.MsgIsPresentedWithItem(PresentedItem) && PresentedItem=="OBJECT_GHOST_GRANNY_NECKLACE") {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            ShownItem=true;
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO") || ShownItem) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            ShownItem=false;
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            {
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (ParentClass.NecklaceReturned) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (ParentClass.GhostToRest) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game->GetHero(),"TEXT_QST_026_HUSBAND_THANKS",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                    } else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game->GetHero(),"TEXT_QST_026_HUSBAND_TELL_MOTHER",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                    }
                } else if (!ParentClass.NecklaceReturned && Game->IsObjectInThingsPossession("OBJECT_GHOST_GRANNY_NECKLACE",Game->GetHero())) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (!ShownItem) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        Game->GiveHeroYesNoQuestion("TEXT_QST_026_RETURN_NECKLACE_QUESTION","TEXT_OBJECT_HERO_ANSWER_YES","TEXT_OBJECT_HERO_ANSWER_NO","",true);
                        long answer;
                        while ((answer=Game->MsgIsQuestionAnsweredYesOrNo())<0) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (answer==0) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                                seh_me.Speak(Game->GetHero(),"TEXT_QST_026_HUSBAND_FIND_GIFT",GROUP_SELECT_NONE,false,true,false);
                                while (seh_me.IsPerformingScriptTask()) {
                                    Game->NewScriptFrame();
                                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                }
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            goto necklace_done;
                        }
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    ParentClass.WifeNeededForCutscene=true;
                    {
                        CScriptThing wife=Game->GetThingWithScriptName("GhostGrannyDaughterInLaw");
                        CScriptGameResourceObjectScriptedThingBase seh_hero;
                        while (!Game->StartScriptingEntity(Game->GetHero(),seh_hero,SCRIPT_AI_PRIORITY_HIGHEST)) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        CScriptGameResourceObjectScriptedThingBase seh_wife;
                        Game->StartScriptingEntity(wife,seh_wife,SCRIPT_AI_PRIORITY_HIGHEST);
                        std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
                        mymap["HERO"]=seh_hero;
                        mymap["WIFE"]=seh_wife;
                        mymap["FARMER"]=seh_me;
                        Game->FixMovieSequenceCamera(true);
                        RunCutsceneMacro("CS_GHOSTGRANNY_OUTRO",mymap,0,0,false,true);
                        Game->FixMovieSequenceCamera(false);
                        Game->TakeObjectFromHero("OBJECT_GHOST_GRANNY_NECKLACE");
                        Game->GiveHeroGold(pGlobalScriptDef->GGN_ReturnNecklaceReward);
                        Game->GiveHeroMorality(pGlobalScriptDef->GGN_ReturnNecklaceMoralityGain);
                        Game->GiveHeroExperience(pGlobalScriptDef->GGN_ReturnNecklaceExperienceGain);
                        ParentClass.NecklaceReturned=true;
                        Game->ClearThingHasInformation(Me);
                    }
                    ParentClass.WifeNeededForCutscene=false;
                    necklace_done:;
                } else if (ParentClass.HeroKnowsAboutNecklace) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game->GetHero(),"TEXT_QST_026_HUSBAND_FIND_GIFT",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (TalkCounter==0) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        ParentClass.WifeNeededForCutscene=true;
                        CScriptThing wife=Game->GetThingWithScriptName("GhostGrannyDaughterInLaw");
                        CScriptGameResourceObjectScriptedThingBase seh_hero;
                        while (!Game->StartScriptingEntity(Game->GetHero(),seh_hero,SCRIPT_AI_PRIORITY_HIGHEST)) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        CScriptGameResourceObjectScriptedThingBase seh_wife;
                        Game->StartScriptingEntity(wife,seh_wife,SCRIPT_AI_PRIORITY_HIGHEST);
                        std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
                        mymap["HERO"]=seh_hero;
                        mymap["WIFE"]=seh_wife;
                        mymap["FARMER"]=seh_me;
                        Game->FixMovieSequenceCamera(true);
                        if (PMasterData->OrchardFarmRaidLastCompleted==2) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            RunCutsceneMacro("CS_GHOSTGRANNY_INTROA",mymap,0,0,false,true);
                        } else {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            RunCutsceneMacro("CS_GHOSTGRANNY_INTROB",mymap,0,0,false,true);
                        }
                    } else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game->GetHero(),"TEXT_QST_026_HUSBAND_OH_MOTHER",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                    }
                    ParentClass.WifeNeededForCutscene=false;
                    if (!ParentClass.QuestCardGiven) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        long answer;
                        if (PMasterData->OrchardFarmRaidLastCompleted==2) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                                seh_me.Speak(Game->GetHero(),"TEXT_QST_026_HUSBAND_QUESTION_A",GROUP_SELECT_NONE,false,true,false);
                                while (seh_me.IsPerformingScriptTask()) {
                                    Game->NewScriptFrame();
                                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                }
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            Game->GiveHeroYesNoQuestion("TEXT_QST_026_HUSBAND_GAME_QUESTION","TEXT_OBJECT_HERO_ANSWER_YES","TEXT_OBJECT_HERO_ANSWER_NO","",true);
                            while ((answer=Game->MsgIsQuestionAnsweredYesOrNo())<0) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                        } else {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                                seh_me.Speak(Game->GetHero(),"TEXT_QST_026_HUSBAND_QUESTION_B",GROUP_SELECT_NONE,false,true,false);
                                while (seh_me.IsPerformingScriptTask()) {
                                    Game->NewScriptFrame();
                                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                }
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            Game->GiveHeroYesNoQuestion("TEXT_QST_026_HUSBAND_GAME_QUESTION","TEXT_OBJECT_HERO_ANSWER_YES","TEXT_OBJECT_HERO_ANSWER_NO","",true);
                            while ((answer=Game->MsgIsQuestionAnsweredYesOrNo())<0) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (answer==1) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            Game->GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_GHOST_GRANNY_NECKLACE",Game->GetActiveQuestName(),false);
                            ParentClass.QuestCardGiven=true;
                            Game->SetQuestCardObjective(Game->GetActiveQuestName(),"TEXT_QUEST_GHOST_GRANNY_NECKLACE_OBJECTIVE_01","OrchardFarm","");
                            if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                                seh_me.Speak(Game->GetHero(),"TEXT_QST_026_HUSBAND_THANKS_FOR_CHOOSING_TO_HELP",GROUP_SELECT_NONE,false,true,false);
                                while (seh_me.IsPerformingScriptTask()) {
                                    Game->NewScriptFrame();
                                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                }
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                        }
                        Game->FixMovieSequenceCamera(false);
                    }
                }
            }
            ++TalkCounter;
            if (seh_me.IsValid()) seh_me.Release();
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (Game->GetHealth(Me)<=1.0f) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                {
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game->StartMovieSequence("",speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game->GetHero(),"TEXT_QST_026_HUSBAND_RUNS_AWAY_01",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    Game->FadeScreenOut(1.0f,1.0f,CRGBColour(0,0,0));
                    Game->Pause(2.0f);
                    Game->RemoveThing(Me,false,false);
                    Game->FadeScreenIn();
                }
            }
        }
    }
}
