// NScript::CQ_NewOakValeIntroScript::CNOVI_Bully::Main() -- the Oakvale bully: walks home, then each frame answers the hero (teddy offer / refusal, intro and taunt lines), counts the hero's hits on a quest health bar until he is subdued and runs off in the BULLYRUN cut-scene, and intimidates the victim when the hero comes near.
// Grade: RELOCATION_MATCH -- actual COFF operands checked against their native targets.
// Inline-budget: guarded predicate depth preserves nested thing calls; implicit scripted handle and donor literal-false outer for loop preserve native cleanup placement.
// Leaf: ?Main@CNOVI_Bully@CQ_NewOakValeIntroScript@NScript@@EAEXXZ
// Retail: 0x00DBB310, 6639 bytes, thiscall.
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
//   Original function-local statics dist (float15 at 0x013AC85C) and freq (long75 at 0x013AC860), proved by donor data/public records and matched load roles.
// Shape: generated entity script on the CNOVI_Victim model (TRetVoid return value, a T-check before every statement).
#define FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CISTHINGALIVE_ORIGINAL_ACCESS
#define FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CPREDICATEBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CPREDICATEBASE_NOVTABLE
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
#include "engine/EScriptEntityMoveType.h"
#include "engine/EHeroAbility.h"
namespace NGlobalConsole { extern bool GDefaultToUsingPhysicsInCutsceneAnimations; }   // 0x01375748
extern CGameScriptInterfaceBase* pDSTGame;                 // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A
bool FABLE_FASTCALL IsDistanceFromThingToPositionOver(const CScriptThing&, const C3DVector&, float);   // 0x00CBE45C
FABLE_FORCEINLINE long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
FABLE_FORCEINLINE CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::map();      // 0x00CDBF70
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::~map();     // 0x00CDBFB0
template<> std::map<CCharString,CCharString>::map();                                     // 0x009AC2D0
template<> std::map<CCharString,CCharString>::~map();                                    // 0x009AC310
inline CScriptThing::~CScriptThing() {}
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
inline NScript::CIsThingConscious::CIsThingConscious(const CScriptThing& thing):Thing(thing) {}
inline NScript::CIsThingConscious::~CIsThingConscious() {}
#ifdef _MSC_VER
#pragma inline_depth()
#endif
void NScript::CQ_NewOakValeIntroScript::CNOVI_Bully::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingConscious(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    {
        C3DVector local_position=Me.GetHomePos();
        float local_range=2.0f;
        EScriptEntityMoveType local_movetype=ENTITY_MOVE_WALK;
        while (IsDistanceFromThingToPositionOver(seh_me.GetScriptThing(),local_position,local_range)) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            seh_me.MoveToPosition(local_position,0.0f,local_movetype,false,true);
            while (seh_me.IsPerformingScriptTask()) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptThing victim=Game.GetThingWithScriptName("NOVI_Victim");
    for (; !false; Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (seh_me.IsValid()) seh_me.Release();
        while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (DoneIntro) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CCharString item_presented;
            if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO") && Game.IsObjectInThingsPossession("OBJECT_TEDDY_BEAR_UNGIVEABLE",Game.GetHero())) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_048_BULLY_FOUND_TEDDY_ONE",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                Game.GiveHeroYesNoQuestion("TEXT_QST_048_GIVE_TEDDY_TO_BULLY","TEXT_OBJECT_HERO_ANSWER_YES","TEXT_OBJECT_HERO_ANSWER_NO","",true);
                long YNQAnswer=Game.MsgIsQuestionAnsweredYesOrNo();
                while (YNQAnswer<0) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    YNQAnswer=Game.MsgIsQuestionAnsweredYesOrNo();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (YNQAnswer==1) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    ParentClass.VictimComplainsAboutLosingTeddy=true;
                    while (ParentClass.VictimComplainsAboutLosingTeddy) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_048_BULLY_FOUND_TEDDY_TWO",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    GivenTeddy();
                    Game.ClearThingHasInformation(Me);
                }
            }
            else if (Me.MsgIsPresentedWithItem(item_presented) && item_presented=="OBJECT_TEDDY_BEAR_UNGIVEABLE") {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                ParentClass.VictimComplainsAboutLosingTeddy=true;
                while (ParentClass.VictimComplainsAboutLosingTeddy) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_048_BULLY_FOUND_TEDDY_TWO",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                GivenTeddy();
                Game.ClearThingHasInformation(Me);
            }
            else if (Me.MsgIsPresentedWithItem(item_presented) && item_presented!="OBJECT_TEDDY_BEAR_UNGIVEABLE") {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_048_BULLY_DONT_WANT",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if (!DoneIntro) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_048_BULLY_GET_LOST",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
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
                        if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game.GetHero(),"TEXT_QST_048_BULLY_IN_COMMON",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game.NewScriptFrame();
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
                            if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                                seh_me.Speak(Game.GetHero(),"TEXT_QST_048_BULLY_NASTY_STREAK",GROUP_SELECT_NONE,false,true,false);
                                while (seh_me.IsPerformingScriptTask()) {
                                    Game.NewScriptFrame();
                                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                }
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                        }
                        else {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                                seh_me.Speak(Game.GetHero(),"TEXT_QST_048_BULLY_DONT_HIT_ME",GROUP_SELECT_NONE,false,true,false);
                                while (seh_me.IsPerformingScriptTask()) {
                                    Game.NewScriptFrame();
                                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                }
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                        }
                    }
                }
                else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_048_BULLY_BADGERING",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
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
                ParentClass.GUIBullyHealthCounter=Game.AddQuestInfoBar((float)InitialHealth,0.0f,CRGBColour(0,255,0,255),CRGBColour(255,0,0,255),"HUD_QUEST_ICON_GRANDSON","",1.0f);
            }
            Game.EntitySetThingAsAllyOfThing(Me,Game.GetHero());
            Game.EntitySetThingAsAllyOfThing(Game.GetHero(),Me);
            if (++HitsTaken<InitialHealth) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
                Game.AddPersonToConversation(HLStyleConversationIndex,victim);
                Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_048_BULLY_SCRMSG_GET_OFF",false,Me,victim);
                Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_048_VICTIM_REVENGE",false,Me,victim);
                Game.UpdateQuestInfoBar(ParentClass.GUIBullyHealthCounter,(float)(InitialHealth-HitsTaken),-1.0f,-1.0f);
            }
            else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                ParentClass.BullySubdued=true;
                Game.RemoveQuestInfoElement(ParentClass.GUIBullyHealthCounter);
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                {
                    CScriptGameResourceObjectScriptedThingBase seh_Hero;
                    if (seh_Hero.IsValid()) seh_Hero.Release();
                    while (!Game.StartScriptingEntity(Game.GetHero(),seh_Hero,SCRIPT_AI_PRIORITY_HIGHEST)) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    CScriptGameResourceObjectScriptedThingBase seh_Victim;
                    if (seh_Victim.IsValid()) seh_Victim.Release();
                    while (!Game.StartScriptingEntity(victim,seh_Victim,SCRIPT_AI_PRIORITY_HIGHEST)) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
                    mymap["HERO"]=seh_Hero;
                    mymap["BRAT"]=seh_Victim;
                    mymap["BULLY"]=seh_me;
                    std::map<CCharString,CCharString> csargs;
                    if (ParentClass.HeroAttackedVictim) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        csargs["$BRATLINE"]="TEXT_QST_048_VICTIM_THANKS_AFTER_HIT";
                    }
                    else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        csargs["$BRATLINE"]="TEXT_QST_048_VICTIM_THANKS";
                    }
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game.StartMovieSequence("",speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                    Game.FixMovieSequenceCamera(true);
                    RunCutsceneMacro("CS_OAKVALEINTRO_BULLYRUN1",mymap,0,&csargs,false,true);
                    if (!ParentClass.GivenHeroTeddy) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        RunCutsceneMacro("CS_OAKVALEINTRO_BULLYRUN2",mymap,0,0,false,true);
                        Game.ClearThingHasInformation(victim);
                        ParentClass.GivenHeroTeddy=true;
                    }
                    else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        RunCutsceneMacro("CS_OAKVALEINTRO_BULLYRUNDUMMY",mymap,0,0,false,true);
                    }
                    Game.FixMovieSequenceCamera(false);
                }
                ParentClass.BullyRanOff=true;
                ParentClass.AddGoodDeed();
                Game.RemoveThing(Me,false,true);
                return _Fail_Val.Return();
            }
        }
        Game.EntitySetFacingAngleTowardsThing(Me,victim,true);
        static float dist=15.0f;
        static long freq=75;
        if (ParentClass.TalkIntermittentTimer.GetTime()==0 && (!SpokenOnFirstProximity || rand()%freq==0) && IsDistanceBetweenThingsUnder(Me,Game.GetHero(),dist) && HitsTaken==0) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            SpokenOnFirstProximity=true;
            ParentClass.TalkIntermittentTimer=3;
            ParentClass.VictimShake=true;
            long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
            Game.AddPersonToConversation(HLStyleConversationIndex,victim);
            CCharString tag;
            tag.Sprintf("TEXT_QST_048_BULLY_SCRMSG_INTIMIDATING_%d",IntimidateSpeechLoop);
            Game.AddLineToConversation(HLStyleConversationIndex,tag,false,Me,victim);
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
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}