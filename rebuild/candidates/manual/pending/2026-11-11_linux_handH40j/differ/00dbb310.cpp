// NScript::CQ_NewOakValeIntroScript::CNOVI_Bully::Main() -- the Oakvale bully: walks home, then each frame answers the hero (teddy offer / refusal, intro and taunt lines), counts the hero's hits on a quest health bar until he is subdued and runs off in the BULLYRUN cut-scene, and intimidates the victim when the hero comes near.
// Grade: DIFFER(6527v6639) -- 141 masked lines, the chunk 811 draft unchanged: the predicate lever of chunk H40 (inline_depth(0) round the CIsThingConscious ctor / dtor + NOVTABLE) builds the same bytes here (the copy is declared only), and adding the row-local inline ~CScriptThing that the walk loop's GetScriptThing temporary needs (retail expands it at 0x00DBB430) gives 386 lines (every other temporary expands too)
// Next idea: expand ~CScriptThing only at the home-walk loop: a row-local FABLE_FORCEINLINE dtor with in-body inline_depth(0) at the sites retail calls, or the loop / guard shape that raises that one site's static frequency (playbook: c1*(1/f+6)/7 < 74)
// Leaf: ?Main@CNOVI_Bully@CQ_NewOakValeIntroScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DBB310, length 6639 bytes, thiscall, no EH frame, ends in its `ret` at 0x00DBCCFE.
// Name: slot 1 of the entity vtable 0x012D879C (0 scalar deleting dtor 0x00DAED10, 2 Init 0x00DAED30, 3 GetParentScript
//   0x00DAED00, all landed as CNOVI_Bully) that Alloc 0x00DAEC60 stores; Ego_r.pdb and FableWin.pdb both hold
//   ?Main@CNOVI_Bully@CQ_NewOakValeIntroScript@NScript@@EAEXXZ.
// Callees: SetPredicate 0x00F35B10 with the CIsThingConscious temporary (vtable 0x012C2FE8), IsActiveThreadTerminating 0x00F35B30,
//   CScriptThing copy 0x004ABE90 / ~ 0x004AA840, scripted-thing handle (ctor 0x007E72A0, IsValid 0x00CD23B9, Release 0x00CD2770,
//   MoveToPosition 0x007E72F0, Speak 0x007E7390, PlayAnimation 0x007E73D0, IsPerformingScriptTask 0x007E7450, GetScriptThing
//   0x007E7490, operator= 0x008ABD10, ~ 0x007E74D0), IsDistanceFromThingToPositionOver 0x00CBE45C, IsDistanceBetweenThingsUnder
//   0x00CBE2FF, MovieBase ctor 0x006E7B60 / dtor 0x006E7B80, cast map ctor 0x00CDBF70 / dtor 0x00CDBFB0 / operator[] 0x00CD3D2E,
//   text map ctor 0x009AC2D0 / dtor 0x009AC310 / operator[] 0x009AC700, RunCutsceneMacro 0x00CBFB7D, CCharString 0x0099E4B0 /
//   0x0099EBF0 / operator=(const char*) 0x0099EFE0 / operator!= 0x0099E960 / Format 0x0099F1F0 / ~ 0x0099EAE0, rand 0x00BFEB16,
//   GivenTeddy 0x00DBCD00, AddGoodDeed 0x00DB0660; interface slots through Game and pDSTGame (TalkIntermittentTimer).
// Data: own InitialHealth +0x1C, HitsTaken +0x20, DoneIntro +0x24, SaidPieceAboutAttackingVictim +0x25, SpokenOnFirstProximity +0x26,
//   IntimidateSpeechLoop +0x28; ParentClass VictimShake +0x48, GUIBullyHealthCounter +0x64, BullySubdued +0x6C, BullyRanOff +0x6D,
//   GivenHeroTeddy +0x6E, HeroAttackedVictim +0x6F, VictimComplainsAboutLosingTeddy +0x96, TalkIntermittentTimer +0x104.
//   Two .data globals (descriptive names: the PDB name sets hold no data symbols): 0x013AC85C 15.0f (the intimidation range),
//   0x013AC860 75 (the 1-in-N repeat chance).
// Shape: generated entity script on the CNOVI_Victim model (TRetVoid return value, a T-check before every statement).
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGNMENT
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_SPRINTF
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CQ_NEWOAKVALEINTROSCRIPT_BULLY
#include <stdlib.h>
#include "engine/C3DVector.h"
#include "engine/CRGBColour.h"
#include "engine/CQ_NewOakValeIntroScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CIsThingConscious.h"
#include "engine/CTimer.h"
#include "engine/fable_script_cutscene.h"
#include "engine/fable_script_distance.h"
#include "engine/TRetVoid.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EScriptEntityMoveType { MOVE_TYPE_WALKING=0 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };
namespace NGlobalConsole { extern bool GDefaultToUsingPhysicsInCutsceneAnimations; }   // 0x01375748
extern CGameScriptInterfaceBase* pDSTGame;                 // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A
extern float g_NOVIBullyIntimidateRange;                    // 0x013AC85C (15.0f), descriptive
extern long g_NOVIBullyIntimidateRepeatChance;              // 0x013AC860 (75), descriptive
bool FABLE_FASTCALL IsDistanceFromThingToPositionOver(const CScriptThing&, const C3DVector&, float);   // 0x00CBE45C
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::map();      // 0x00CDBF70
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::~map();     // 0x00CDBFB0
template<> std::map<CCharString,CCharString>::map();                                     // 0x009AC2D0
template<> std::map<CCharString,CCharString>::~map();                                    // 0x009AC310
inline NScript::CIsThingConscious::CIsThingConscious(const CScriptThing& thing):Thing(thing) {}
inline NScript::CIsThingConscious::~CIsThingConscious() {}
void NScript::CQ_NewOakValeIntroScript::CNOVI_Bully::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingConscious(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    {
        C3DVector home_pos=Me.GetHomePos();
        while (IsDistanceFromThingToPositionOver(seh_me.GetScriptThing(),home_pos,2.0f)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            seh_me.MoveToPosition(home_pos,0.0f,MOVE_TYPE_WALKING,false,true);
            while (seh_me.IsPerformingScriptTask()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptThing Victim=Game->GetThingWithScriptName("NOVI_Victim");
    for (; !IsActiveThreadTerminating(); Game->NewScriptFrame()) {
        if (seh_me.IsValid()) seh_me.Release();
        while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (DoneIntro) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CCharString item;
            if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO") && Game->IsObjectInThingsPossession("OBJECT_TEDDY_BEAR_UNGIVEABLE",Game->GetHero())) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game->GetHero(),"TEXT_QST_048_BULLY_FOUND_TEDDY_ONE",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                Game->GiveHeroYesNoQuestion("TEXT_QST_048_GIVE_TEDDY_TO_BULLY","TEXT_OBJECT_HERO_ANSWER_YES","TEXT_OBJECT_HERO_ANSWER_NO","",true);
                long answer=Game->MsgIsQuestionAnsweredYesOrNo();
                while (answer<0) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    answer=Game->MsgIsQuestionAnsweredYesOrNo();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (answer==1) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    ParentClass.VictimComplainsAboutLosingTeddy=true;
                    while (ParentClass.VictimComplainsAboutLosingTeddy) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game->GetHero(),"TEXT_QST_048_BULLY_FOUND_TEDDY_TWO",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    GivenTeddy();
                    Game->ClearThingHasInformation(Me);
                }
            }
            else if (Me.MsgIsPresentedWithItem(item) && item=="OBJECT_TEDDY_BEAR_UNGIVEABLE") {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                ParentClass.VictimComplainsAboutLosingTeddy=true;
                while (ParentClass.VictimComplainsAboutLosingTeddy) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game->GetHero(),"TEXT_QST_048_BULLY_FOUND_TEDDY_TWO",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                GivenTeddy();
                Game->ClearThingHasInformation(Me);
            }
            else if (Me.MsgIsPresentedWithItem(item) && item!="OBJECT_TEDDY_BEAR_UNGIVEABLE") {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game->GetHero(),"TEXT_QST_048_BULLY_DONT_WANT",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (!DoneIntro) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game->GetHero(),"TEXT_QST_048_BULLY_GET_LOST",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                DoneIntro=true;
            }
            else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (ParentClass.HeroAttackedVictim) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (!SaidPieceAboutAttackingVictim) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game->GetHero(),"TEXT_QST_048_BULLY_IN_COMMON",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        SaidPieceAboutAttackingVictim=true;
                    }
                    else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (HitsTaken<3) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                                seh_me.Speak(Game->GetHero(),"TEXT_QST_048_BULLY_NASTY_STREAK",GROUP_SELECT_NONE,false,true,false);
                                while (seh_me.IsPerformingScriptTask()) {
                                    Game->NewScriptFrame();
                                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                }
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                        }
                        else {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                                seh_me.Speak(Game->GetHero(),"TEXT_QST_048_BULLY_DONT_HIT_ME",GROUP_SELECT_NONE,false,true,false);
                                while (seh_me.IsPerformingScriptTask()) {
                                    Game->NewScriptFrame();
                                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                }
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                        }
                    }
                }
                else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game->GetHero(),"TEXT_QST_048_BULLY_BADGERING",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
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
            if (ParentClass.GUIBullyHealthCounter==-999) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                ParentClass.GUIBullyHealthCounter=Game->AddQuestInfoBar((float)InitialHealth,0.0f,CRGBColour(0,255,0,255),CRGBColour(255,0,0,255),"HUD_QUEST_ICON_GRANDSON","",1.0f);
            }
            Game->EntitySetThingAsAllyOfThing(Me,Game->GetHero());
            Game->EntitySetThingAsAllyOfThing(Game->GetHero(),Me);
            if (++HitsTaken<InitialHealth) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                long conversation=Game->AddNewConversation(Me,false,false);
                Game->AddPersonToConversation(conversation,Victim);
                Game->AddLineToConversation(conversation,"TEXT_QST_048_BULLY_SCRMSG_GET_OFF",false,Me,Victim);
                Game->AddLineToConversation(conversation,"TEXT_QST_048_VICTIM_REVENGE",false,Me,Victim);
                Game->UpdateQuestInfoBar(ParentClass.GUIBullyHealthCounter,(float)(InitialHealth-HitsTaken),-1.0f,-1.0f);
            }
            else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                ParentClass.BullySubdued=true;
                Game->RemoveQuestInfoElement(ParentClass.GUIBullyHealthCounter);
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                {
                    CScriptGameResourceObjectScriptedThingBase seh_hero;
                    if (seh_hero.IsValid()) seh_hero.Release();
                    while (!Game->StartScriptingEntity(Game->GetHero(),seh_hero,SCRIPT_AI_PRIORITY_HIGHEST)) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    CScriptGameResourceObjectScriptedThingBase seh_victim;
                    if (seh_victim.IsValid()) seh_victim.Release();
                    while (!Game->StartScriptingEntity(Victim,seh_victim,SCRIPT_AI_PRIORITY_HIGHEST)) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
                    mymap["HERO"]=seh_hero;
                    mymap["BRAT"]=seh_victim;
                    mymap["BULLY"]=seh_me;
                    std::map<CCharString,CCharString> textmap;
                    if (ParentClass.HeroAttackedVictim) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        textmap["$BRATLINE"]="TEXT_QST_048_VICTIM_THANKS_AFTER_HIT";
                    }
                    else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        textmap["$BRATLINE"]="TEXT_QST_048_VICTIM_THANKS";
                    }
                    CScriptGameResourceObjectMovieBase movie;
                    Game->StartMovieSequence("",movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    Game->FixMovieSequenceCamera(true);
                    RunCutsceneMacro("CS_OAKVALEINTRO_BULLYRUN1",mymap,0,&textmap,false,true);
                    if (!ParentClass.GivenHeroTeddy) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        RunCutsceneMacro("CS_OAKVALEINTRO_BULLYRUN2",mymap,0,0,false,true);
                        Game->ClearThingHasInformation(Victim);
                        ParentClass.GivenHeroTeddy=true;
                    }
                    else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        RunCutsceneMacro("CS_OAKVALEINTRO_BULLYRUNDUMMY",mymap,0,0,false,true);
                    }
                    Game->FixMovieSequenceCamera(false);
                }
                ParentClass.BullyRanOff=true;
                ParentClass.AddGoodDeed();
                Game->RemoveThing(Me,false,true);
                return _Fail_Val.Return();
            }
        }
        Game->EntitySetFacingAngleTowardsThing(Me,Victim,true);
        if (ParentClass.TalkIntermittentTimer.GetTime()==0 && (!SpokenOnFirstProximity || rand()%g_NOVIBullyIntimidateRepeatChance==0) && IsDistanceBetweenThingsUnder(Me,Game->GetHero(),g_NOVIBullyIntimidateRange) && HitsTaken==0) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            SpokenOnFirstProximity=true;
            ParentClass.TalkIntermittentTimer=3;
            ParentClass.VictimShake=true;
            long conversation=Game->AddNewConversation(Me,false,false);
            Game->AddPersonToConversation(conversation,Victim);
            CCharString line;
            line.Sprintf("TEXT_QST_048_BULLY_SCRMSG_INTIMIDATING_%d",IntimidateSpeechLoop);
            Game->AddLineToConversation(conversation,line,false,Me,Victim);
            IntimidateSpeechLoop+=10;
            if (IntimidateSpeechLoop>40) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                IntimidateSpeechLoop=10;
            }
            if (rand()%2==0) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                seh_me.PlayAnimation("ST_OPINION_DISAPPROVAL_SHAKE_FIST",false,false,false,true,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
            }
            else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                seh_me.PlayAnimation("ST_OPINION_DISAPPROVAL_POINT_AT",false,false,false,true,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
            }
        }
    }
}
