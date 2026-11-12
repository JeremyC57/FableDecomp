// NScript::CQ_NewOakValeIntroScript::CNOVI_Theresa::Main() -- Theresa at the Oakvale intro: waits for the hero, plays the meeting cut-scene (offering the chocolate box), then answers talk, presents and hits until she has the chocolates and the hero reaches M_TriggerOutro, which plays the raid cut-scene and video
// Grade: DIFFER(6859v7013) -- 125 masked lines, 37 hunks (248 before; chunk H40j, 9 grades). Levers: the outro ends in `break;` and a post-loop `if (IsActiveThreadTerminating()) return _Fail_Val.Return();` follows (retail tests T after ~item, 0x00DBB2E1); the main loop is `for (;; NSF) { if (IsActiveThreadTerminating() || false) return ...;`. Left: the zero-v-vtable register (as 0x00E50FD0) and the return-pad cross-jump layout (retail keeps a private epilogue pad per exit).
// Next idea: tools/xj.sh on the exit pads: find the statement both arms own that keeps retail's pads apart (the 0x00E64FB0 cross-jump lever). Byte-neutral in H40j: goto label, outro as fall-through, implicit MovieBase / STB dtors (no OUTOFLINE_DTOR); worse: without CPREDICATEBASE_NOVTABLE (239), `for (;!T||false;NSF)` (838). H40k (5 grades): baseline 125; /d2inlT75 neutral, /d2inlT73 worse (184); `(given && dist) || false` and `given && dist && true` on the outro test byte-identical. Measured: retail lays the outro (0x00DBB0E4..) directly before the exit block 0x00DBB2E8 and ends it with a bare `call IsActiveThreadTerminating` (post-loop if/return folded), so the T-return pads (`call ~movie ; ~item ; jmp 0x00DBB2E8`) have no fall-through twin and stay apart; ours puts the exit block (0x00DBB045) before the outro, right after the last pad, and c2 cross-jumps every pad into that pad's tail. Next: find what makes c2 place the outro before the exit (not branch frequency).
// Leaf: ?Main@CNOVI_Theresa@CQ_NewOakValeIntroScript@NScript@@EAEXXZ
// Identity: Ego_r.pdb / FableWin.pdb ?Main@CNOVI_Theresa@CQ_NewOakValeIntroScript@NScript@@EAEXXZ (private virtual); entity vtable 0x012D83A4
//   slot 1 = 0x00DB97A0 (its only .rdata reference 0x012D83A8), stored by the Alloc 0x00DAC420; slot 0 the landed scalar deleting dtor
//   0x00DAC4D0, slot 2 the landed Init 0x00DAC4F0.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DB97A0, 7013 bytes, no jump table, no EH frame.
// Layout: engine/CQ_NewOakValeIntroScript.h, opt-in FABLE_RETAIL_CQ_NEWOAKVALEINTROSCRIPT_BARRELMAN_THERESA (this chunk's overlay).
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
    CScriptGameResourceObjectScriptedThingBase seh_me;
    bool given_chocolates = false;
    CScriptThing outro_trigger = Game->GetThingWithScriptName("M_TriggerOutro");
    for (;; Game->NewScriptFrame()) {
        if (IsActiveThreadTerminating() || false) return _Fail_Val.Return();
        if (seh_me.IsValid()) seh_me.Release();
        while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (!DoneIntro) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            for (;; Game->NewScriptFrame()) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
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
            CScriptGameResourceObjectMovieBase movie;
            Game->StartMovieSequence("",movie);
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
                    std::vector<CScriptThing> guards;
                    Game->GetAllThingsWithScriptName("NOVI_Guard",guards);
                    KillAllThingsInVector(*Game,guards,false);
                    std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> yesmap;
                    yesmap["HERO"]=seh_hero;
                    yesmap["THER"]=seh_me;
                    RunCutsceneMacro("CS_OAKVALE_INTRO_THERESA_MEET_YES",yesmap,0,0,false,true);
                    ParentClass.GivenTheresaChocs=true;
                    Game->TakeObjectFromHero("OBJECT_CHOCOLATE_BOX_UNGIVEABLE");
                    Game->SetQuestCardObjective(Game->GetActiveQuestName(),"TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_05","","");
                    given_chocolates=true;
                    Game->ClearThingHasInformation(Me);
                }
            }
            Game->FixMovieSequenceCamera(false);
            DoneIntro=true;
        } else {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CCharString item;
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
                    std::vector<CScriptThing> guards;
                    Game->GetAllThingsWithScriptName("NOVI_Guard",guards);
                    KillAllThingsInVector(*Game,guards,false);
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
                    given_chocolates=true;
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
            } else if (Me.MsgIsPresentedWithItem(item) && item=="OBJECT_CHOCOLATE_BOX_UNGIVEABLE") {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                std::vector<CScriptThing> guards;
                Game->GetAllThingsWithScriptName("NOVI_Guard",guards);
                KillAllThingsInVector(*Game,guards,false);
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
                given_chocolates=true;
                Game->ClearThingHasInformation(Me);
            } else if (Me.MsgIsPresentedWithItem(item) && item!="OBJECT_CHOCOLATE_BOX_UNGIVEABLE") {
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
                if (given_chocolates) {
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
            } else if (!given_chocolates && !seh_me.IsPerformingScriptTask()) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                seh_me.PlayCombatAnimation("SKIP",true,false,false,true,false,false);
            }
            if (given_chocolates && IsDistanceBetweenThingsUnder(Game->GetHero(),outro_trigger,2.0f)) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game->DisplayQuestInfo(false);
                Game->RemoveQuestInfoElement(ParentClass.GUIBullyHealthCounter);
                Game->RemoveQuestInfoElement(ParentClass.GUIGoodDeedCounter);
                Game->RemoveQuestInfoElement(ParentClass.GUIBarrelCounter);
                CScriptGameResourceObjectMovieBase movie;
                Game->StartMovieSequence("",movie);
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
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}
