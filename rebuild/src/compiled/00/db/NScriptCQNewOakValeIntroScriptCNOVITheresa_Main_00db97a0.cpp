// NScript::CQ_NewOakValeIntroScript::CNOVI_Theresa::Main() -- Theresa (the hero's sister) at the Oakvale intro: waits for the hero, plays the meeting cut-scene (offering the chocolate box), then answers talk, presents and hits until she has the chocolates and the hero reaches M_TriggerOutro, which plays the raid cut-scene and video and ends the attack.
// Grade: RELOCATION_MATCH (7013v7013) -- wfE1, 2026-11-14 (from the chunk H40j/H40k draft, 6859v7013).
// Inline-budget: two scoped `#pragma inline_depth(0)` ... `#pragma inline_depth()` regions -- (1) round the terminate check at the top
//   of the intro's `for (;; NSF)` loop: retail CALLS the ~CScriptGameResourceObjectScriptedThingBase / ~CScriptThing exit copies there
//   (0x00DB98ED jne 0x00DBB2E8, the all-call exit block) while it EXPANDS the main-loop-top (0x00DB985C -> pad 0x00DBA426, c2 inline
//   score 56) and the StartScriptingEntity-wait exits (0x00DB98A1 -> pad 0x00DBAE56, score 65); the intro-loop site scores 73 (expanded at
//   the default 74), so no single /d2inlT separates them (73 lies between the two expanded sites' 56/65 and nothing; T<=73 also calls
//   site 65's nested CCountedPointer dtor at T<=50). (2) round the post-loop terminate check and the closing brace of the block that
//   owns seh_me / outro_trigger: retail calls their destructors at the function's exit block 0x00DBB2E8 (c2 score 47 at f 2.50, after
//   the `for (; true; ...)` loop) -- a per-region device because the region cannot end at the function's own closing brace (an
//   inline_depth value in force there applies to the whole function, lesson wfC2 0x00F0FB70).
// FITTED: the `{ }` block from seh_me's declaration to the end (carries pragma region (2); without it 6967v7013, 27 hunks), and the
//   main loop spelling `for (; true; Game->NewScriptFrame())` (VC7.1 halves the static frequency of the loop body and after it,
//   lesson wfD2 0x00EE57B0; `for (;;` = 7223v7013, `while (true)` with the frame call at the bottom = the same bytes as this).
// EVIDENCED: local names from the Anniversary PDB S_REGREL32 list of CNOVI_Theresa::Main (leaving_sister_switch = the one bool local,
//   byte +0x17; seh_me, seh_hero, speech_movie, mymap, nearby_guards, item_presented); outro_trigger is descriptive (no name there).
// Leaf: ?Main@CNOVI_Theresa@CQ_NewOakValeIntroScript@NScript@@EAEXXZ
// Identity: Ego_r.pdb / FableWin.pdb ?Main@CNOVI_Theresa@CQ_NewOakValeIntroScript@NScript@@EAEXXZ (private virtual); entity vtable 0x012D83A4
//   slot 1 = 0x00DB97A0 (its only .rdata reference 0x012D83A8), stored by the Alloc 0x00DAC420; slot 0 the landed scalar deleting dtor
//   0x00DAC4D0, slot 2 the landed Init 0x00DAC4F0.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DB97A0, 7013 bytes (ends in its `ret` at 0x00DBB304), thiscall, no jump table, no EH frame.
// Layout: engine/CQ_NewOakValeIntroScript.h, repo opt-in FABLE_RETAIL_CQ_NEWOAKVALEINTROSCRIPT_BARRELMAN_THERESA.
#define FABLE_RETAIL_CPREDICATEBASE_NOVTABLE
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGNMENT
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CQ_NEWOAKVALEINTROSCRIPT_BARRELMAN_THERESA
#include <string.h>
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CQ_NewOakValeIntroScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CIsThingConscious.h"
#include "engine/CRGBColour.h"
#include "engine/EMusicSetType.h"
#include "engine/fable_script_cutscene.h"
#include "engine/fable_script_distance.h"
#include "engine/TRetVoid.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::map();     // 0x00CDBF70
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::~map();    // 0x00CDBFB0
template<> std::vector<CScriptThing>::~vector();                                        // 0x008AC970
void FABLE_FASTCALL KillAllThingsInVector(CGameScriptInterfaceBase&, const std::vector<CScriptThing>&, bool);   // 0x00CBED82
inline CScriptThing::~CScriptThing() {}
#pragma inline_depth(0)
inline NScript::CIsThingConscious::CIsThingConscious(const CScriptThing& thing):Thing(thing) {}
#pragma inline_depth()
#pragma inline_depth(0)
inline NScript::CIsThingConscious::~CIsThingConscious() {}
#pragma inline_depth()

void NScript::CQ_NewOakValeIntroScript::CNOVI_Theresa::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingConscious(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    {
        CScriptGameResourceObjectScriptedThingBase seh_me;
        bool leaving_sister_switch = false;
        CScriptThing outro_trigger = Game->GetThingWithScriptName("M_TriggerOutro");
        for (; true; Game->NewScriptFrame()) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (!DoneIntro) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                for (;; Game->NewScriptFrame()) {
#pragma inline_depth(0)
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
#pragma inline_depth()
                    seh_me.PlayCombatAnimation("SKIP",true,false,false,true,false,false);
                    while (!IsDistanceBetweenThingsUnder(Me,Game->GetHero(),5.0f) && seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (IsDistanceBetweenThingsUnder(Me,Game->GetHero(),5.0f)) break;
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectScriptedThingBase seh_hero;
                Game->StartScriptingEntity(Game->GetHero(),seh_hero,SCRIPT_AI_PRIORITY_HIGHEST);
                std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
                mymap["HERO"]=seh_hero;
                mymap["THER"]=seh_me;
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                Game->FixMovieSequenceCamera(true);
                RunCutsceneMacro("CS_OAKVALE_INTRO_THERESA_MEET",mymap,0,0,false,true);
                if (Game->IsObjectInThingsPossession("OBJECT_CHOCOLATE_BOX_UNGIVEABLE",Game->GetHero())) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game->GiveHeroYesNoQuestion("TEXT_QST_048_GIVE_CHOCOLATE_BOX","TEXT_OBJECT_HERO_ANSWER_YES","TEXT_OBJECT_HERO_ANSWER_NO","",true);
                    long answer=Game->MsgIsQuestionAnsweredYesOrNo();
                    while (answer<0) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        answer=Game->MsgIsQuestionAnsweredYesOrNo();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (answer==1) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        std::vector<CScriptThing> nearby_guards;
                        Game->GetAllThingsWithScriptName("NOVI_Guard",nearby_guards);
                        KillAllThingsInVector(*Game,nearby_guards,false);
                        std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
                        mymap["HERO"]=seh_hero;
                        mymap["THER"]=seh_me;
                        RunCutsceneMacro("CS_OAKVALE_INTRO_THERESA_MEET_YES",mymap,0,0,false,true);
                        ParentClass.GivenTheresaChocs=true;
                        Game->TakeObjectFromHero("OBJECT_CHOCOLATE_BOX_UNGIVEABLE");
                        Game->SetQuestCardObjective(Game->GetActiveQuestName(),"TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_05","","");
                        leaving_sister_switch=true;
                        Game->ClearThingHasInformation(Me);
                    }
                }
                Game->FixMovieSequenceCamera(false);
                DoneIntro=true;
            } else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CCharString item_presented;
                if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO") && Game->IsObjectInThingsPossession("OBJECT_CHOCOLATE_BOX_UNGIVEABLE",Game->GetHero())) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game->StartMovieSequence("",speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    Game->GiveHeroYesNoQuestion("TEXT_QST_048_GIVE_CHOCOLATE_BOX","TEXT_OBJECT_HERO_ANSWER_YES","TEXT_OBJECT_HERO_ANSWER_NO","",true);
                    long answer=Game->MsgIsQuestionAnsweredYesOrNo();
                    while (answer<0) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        answer=Game->MsgIsQuestionAnsweredYesOrNo();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (answer==1) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        std::vector<CScriptThing> nearby_guards;
                        Game->GetAllThingsWithScriptName("NOVI_Guard",nearby_guards);
                        KillAllThingsInVector(*Game,nearby_guards,false);
                        if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game->GetHero(),"TEXT_QST_048_THERESA_HELLO",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        ParentClass.GivenTheresaChocs=true;
                        Game->TakeObjectFromHero("OBJECT_CHOCOLATE_BOX_UNGIVEABLE");
                        Game->SetQuestCardObjective(Game->GetActiveQuestName(),"TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_05","","");
                        leaving_sister_switch=true;
                        Game->ClearThingHasInformation(Me);
                    } else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game->GetHero(),"TEXT_QST_048_THERESA_REALLY_GET_PRESENT",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                    }
                } else if (Me.MsgIsPresentedWithItem(item_presented) && item_presented=="OBJECT_CHOCOLATE_BOX_UNGIVEABLE") {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game->StartMovieSequence("",speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    std::vector<CScriptThing> nearby_guards;
                    Game->GetAllThingsWithScriptName("NOVI_Guard",nearby_guards);
                    KillAllThingsInVector(*Game,nearby_guards,false);
                    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game->GetHero(),"TEXT_QST_048_THERESA_HELLO",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    ParentClass.GivenTheresaChocs=true;
                    Game->TakeObjectFromHero("OBJECT_CHOCOLATE_BOX_UNGIVEABLE");
                    Game->SetQuestCardObjective(Game->GetActiveQuestName(),"TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_05","","");
                    leaving_sister_switch=true;
                    Game->ClearThingHasInformation(Me);
                } else if (Me.MsgIsPresentedWithItem(item_presented) && item_presented!="OBJECT_CHOCOLATE_BOX_UNGIVEABLE") {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game->StartMovieSequence("",speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game->GetHero(),"TEXT_QST_048_THERESA_BETTER_PRESENT",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                } else if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game->StartMovieSequence("",speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    if (leaving_sister_switch) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game->GetHero(),"TEXT_QST_048_THERESA_HELLO",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                    } else if (!spoken_already) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        spoken_already=true;
                        if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game->GetHero(),"TEXT_QST_048_THERESA_GET_PRESENT",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                    } else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game->GetHero(),"TEXT_QST_048_THERESA_REALLY_GET_PRESENT",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                    }
                } else if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game->EntitySetThingAsAllyOfThing(Me,Game->GetHero());
                    Game->EntitySetThingAsAllyOfThing(Game->GetHero(),Me);
                    ParentClass.AddBadDeed((CQ_NewOakValeIntroScript::EBadDeeds)2);
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game->StartMovieSequence("",speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game->GetHero(),"TEXT_QST_048_THERESA_DONT_HIT",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                } else if (!leaving_sister_switch && !seh_me.IsPerformingScriptTask()) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    seh_me.PlayCombatAnimation("SKIP",true,false,false,true,false,false);
                }
                if (leaving_sister_switch && IsDistanceBetweenThingsUnder(Game->GetHero(),outro_trigger,2.0f)) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game->DisplayQuestInfo(false);
                    Game->RemoveQuestInfoElement(ParentClass.GUIBullyHealthCounter);
                    Game->RemoveQuestInfoElement(ParentClass.GUIGoodDeedCounter);
                    Game->RemoveQuestInfoElement(ParentClass.GUIBarrelCounter);
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game->StartMovieSequence("",speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    CScriptGameResourceObjectScriptedThingBase seh_hero;
                    Game->StartScriptingEntity(Game->GetHero(),seh_hero,SCRIPT_AI_PRIORITY_HIGHEST);
                    std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
                    mymap["HERO"]=seh_hero;
                    mymap["Theresa"]=seh_me;
                    RunCutsceneMacro("CS_OAKVALE_INTRO_THERESA",mymap,0,0,false,true);
                    Game->PlayAVIMovie("Data\\Video\\1_raid_on_oak_vale_comp.xmv");
                    Game->FadeScreenOut(0.5f,0.0f,CRGBColour(0,0,0,255));
                    Game->OverrideMusic(MUSIC_SET_PRISON,false,false);
                    ParentClass.AttackOver=true;
                    break;
                }
            }
        }
#pragma inline_depth(0)
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
#pragma inline_depth()
}