// NScript::CV_IntroductionToTrophiesScript::CBriarRose::Main() -- Briar Rose in the tavern (Introduction to Trophies): face the M_TrophiesLookAtMe marker, then each frame switch her opinion-source loudness to the required level, warn the hero once he has hit her too often, play a random idle animation when her anim timer runs out, let a tavern watcher comment as the hero approaches, answer when talked to (sniffy after an attack, the CS_TROPHIES_INTRO / CS_TROPHIES_TROPHY cut scenes, then blather), give the attack speech after the first attack, and post the trophy-show opinion deed on a timer
// Grade: DIFFER(6035v5915) -- per-exit dtor splits: retail expands ~CTimer / ~CScriptThing / the movie dtor at the natural exit and the outer T pads but calls 0x00CD4470 / 0x004AA840 / 0x006E7B80 at the shared nested pad (ours expands one site more of each); the in-loop LookAtMe.IsAlive is a call 0x004AB130 in retail (ours expands it, the pre-loop one is expanded in both); ours keeps 0 in ebp where retail pushes immediates, so the pads merge differently (440 difflines)
// Next idea: the per-site inline split is the chunk 807 0x00EE57B0 ~CTimer park (loop spellings, declared-only and noinline all tried); a lane-wide answer to VC7.1 per-site inline choice (docs/pipeline/INLINE_DEPTH_REVISIT.md), else leave parked
// Leaf: ?Main@CBriarRose@CV_IntroductionToTrophiesScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E9E420, thiscall, no EH frame.
// Identity (proven, two sources): the address appears once in .rdata, at slot 1 of the entity vtable 0x012ED67C stored by
//   CBriarRose::Alloc 0x00E9D9D0 (slot 0 the landed scalar deleting dtor 0x00E9DA90, slot 2 the landed Init 0x00E9DAC0, slot 3 the
//   landed GetParentScript 0x00E9DA70); Ego_r.pdb and FableWin.pdb both hold ?Main@CBriarRose@CV_IntroductionToTrophiesScript@NScript@@EAEXXZ.
// Callees: CTimer (pDSTGame Register / Deregister / Set / GetTimer), CScriptThing ctor (inline) / copy 0x004ABE90 / ~ 0x004AA840 /
//   IsAlive 0x004AB130, SetPredicate 0x00F35B10 with the CIsThingAlive temporary (dtor 0x00E241B0), IsActiveThreadTerminating
//   0x00F35B30, the scripted-thing handle (CBaseObject 0x0099A380, ctor 0x007E72A0, IsValid 0x00CD23B9, Release 0x00CD2770,
//   ClearCommands 0x007E7360, PlayAnimation 0x007E73D0, PlayLoopingAnimation 0x007E73F0, Speak 0x007E7390, IsPerformingScriptTask
//   0x007E7450, GetScriptThing 0x007E7490, operator= 0x008ABD10, ~ 0x007E74D0), MovieBase ctor 0x006E7B60 / dtor 0x006E7B80, cast
//   map ctor 0x00CDBF70 / dtor 0x00CDBFB0 / operator[] 0x00CD3D2E, RunCutsceneMacro 0x00CBFB7D, IsDistanceBetweenThingsUnder
//   0x00CBE2FF, rand 0x00BFEB16, CCharString 0x0099EBF0 / ~ 0x0099EAE0; NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations
//   0x01375748; interface slots through Game.
// Data: engine/CV_IntroductionToTrophiesScript.h (new, chunk 809); pGlobalScriptDef TrophiesOpinionDeedTimer +0x69c,
//   TrophiesBriarRoseProximityNear / Far +0x6a8 / +0x6ac, TrophiesBriarRoseAnimTime / Var +0x6c4 / +0x6c8,
//   TrophiesHitCountForBriarAttack +0x6d4.
// Shape: generated entity script; three function-scope locals the body never uses (a timer, a CScriptThing, a timer), the main loop
//   `for (;; NewScriptFrame())` with the T-check at the top of the body (the OpinionDeedTimer index then stays in ebx, as retail).
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#include "engine/C3DVector.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CV_IntroductionToTrophiesScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/fable_script_cutscene.h"
#include "engine/fable_script_distance.h"
#include "engine/fable_script_animation_defaults.h"
#include "engine/EOpinionDeedType.h"
#include "engine/CScriptDef.h"
#include "engine/CDefPointer.h"
#include "engine/CTimer.h"
#include "engine/TRetVoid.h"
#include "engine/CIsThingAlive.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
extern "C" int __cdecl rand();
extern CGameScriptInterfaceBase* pDSTGame;
extern CDefPointer<const CScriptDef> pGlobalScriptDef;
inline CTimer::CTimer() { TimerIndex = pDSTGame->RegisterTimer(); }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
inline CScriptThing::CScriptThing() {}
inline CScriptThing::~CScriptThing() {}
inline bool CScriptThing::IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::map();
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::~map();
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
void NScript::CV_IntroductionToTrophiesScript::CBriarRose::Main()
{
    TRetVoid _Fail_Val;
    CTimer OpinionDeedTimer;
    CScriptThing UnusedThing;
    CTimer UnusedTimer;
    AnimTimer=0;
    CScriptThing LookAtMe=Game->GetThingWithScriptName("M_TrophiesLookAtMe");
    if (LookAtMe.IsAlive()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->EntitySetFacingAngleTowardsThing(Me,LookAtMe,false);
    }
    OpinionDeedTimer=pGlobalScriptDef->TrophiesOpinionDeedTimer;
    SetPredicate(CIsThingAlive(Me));
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    for (;; Game->NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (ParentClass.OpinionCurrent!=ParentClass.OpinionRequired) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (ParentClass.OpinionRequired==0) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game->EntitySetAsOpinionSource(Me,"OPINION_SOURCE_BRIAR_ROSE_LOUD");
                ParentClass.OpinionCurrent=(BriarRoseOpinion)0;
            } else if (ParentClass.OpinionRequired==1) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game->EntitySetAsOpinionSource(Me,"OPINION_SOURCE_BRIAR_ROSE_MEDIUM");
                ParentClass.OpinionCurrent=(BriarRoseOpinion)1;
            } else if (ParentClass.OpinionRequired==2) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game->EntitySetAsOpinionSource(Me,"OPINION_SOURCE_BRIAR_ROSE_QUIET");
                ParentClass.OpinionCurrent=(BriarRoseOpinion)2;
            }
        }
        if (ParentClass.HitByHeroCount>pGlobalScriptDef->TrophiesHitCountForBriarAttack && ParentClass.AttackSpeechComplete && !ParentClass.HitWarningComplete) {
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
                seh_me.Speak(Game->GetHero(),"TEXT_QST_038_BRIAR_WARNING",GROUP_SELECT_RANDOM_NO_REPEAT,false,true,false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            ParentClass.HitWarningComplete=true;
        }
        if (AnimTimer.GetTime()==0 && seh_me.IsValid()) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            seh_me.ClearCommands();
            if (LookAtMe.IsAlive()) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game->EntitySetFacingAngleTowardsThing(Me,LookAtMe,false);
            }
            while (seh_me.IsPerformingScriptTask()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            long animation=rand()%8;
            if (animation==0) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                seh_me.PlayAnimation("ST_IDLE",false,false,true,false,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
            } else if (animation==1) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                seh_me.PlayAnimation("ST_IDLE_SUBTLE",false,false,true,false,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
            } else if (animation==2) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                seh_me.PlayAnimation("SCRIPT_TALK_IDLE_01",false,false,true,false,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
            } else if (animation==3) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                seh_me.PlayAnimation("SCRIPT_TALK_IDLE_02",false,false,true,false,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
            } else if (animation==4) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                seh_me.PlayAnimation("SCRIPT_TALK_IDLE_03",false,false,true,false,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
            } else if (animation==5) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            } else if (animation==6) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                seh_me.PlayAnimation("SCRIPT_LISTEN_BASE_IN",false,false,true,false,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
                seh_me.PlayLoopingAnimation("SCRIPT_LISTEN_BASE_LOOP",1,false,false,true,false,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
                seh_me.PlayAnimation("SCRIPT_LISTEN_BASE_OUT",false,false,true,true,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
            } else if (animation==7) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                seh_me.PlayAnimation("SCRIPT_FOLD_ARMS_IN",false,false,true,false,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
                seh_me.PlayLoopingAnimation("SCRIPT_FOLD_ARMS_LOOP",1,false,false,true,false,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
                seh_me.PlayAnimation("SCRIPT_FOLD_ARMS_OUT",false,false,true,true,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
            }
            AnimTimer=rand()%pGlobalScriptDef->TrophiesBriarRoseAnimTimeVar+pGlobalScriptDef->TrophiesBriarRoseAnimTime;
        }
        if (Game->GetHero().IsAlive() && ParentClass.InsideTavern) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (!ParentClass.DoneApproachMessage1 && IsDistanceBetweenThingsUnder(Me,Game->GetHero(),pGlobalScriptDef->TrophiesBriarRoseProximityFar)) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptThing watcher=Game->GetRandomThingWithScriptName("TrophiesIntroWatcher");
                if (watcher.IsAlive()) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    long conversation=Game->AddNewConversation(watcher,false,false);
                    Game->AddPersonToConversation(conversation,Game->GetHero());
                    Game->AddLineToConversation(conversation,"TEXT_QST_038_BRIAR_APPROACH_01",false,watcher,Game->GetHero());
                }
                ParentClass.DoneApproachMessage1=true;
            }
            if (!ParentClass.DoneApproachMessage2 && IsDistanceBetweenThingsUnder(Me,Game->GetHero(),pGlobalScriptDef->TrophiesBriarRoseProximityNear)) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptThing watcher=Game->GetRandomThingWithScriptName("TrophiesIntroWatcher");
                if (watcher.IsAlive()) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    long conversation=Game->AddNewConversation(watcher,false,false);
                    Game->AddPersonToConversation(conversation,Game->GetHero());
                    Game->AddLineToConversation(conversation,"TEXT_QST_038_BRIAR_APPROACH_02",false,watcher,Game->GetHero());
                }
                ParentClass.DoneApproachMessage2=true;
            }
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            seh_me.ClearCommands();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            while (seh_me.IsPerformingScriptTask()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (ParentClass.AttackedByHero && ParentClass.AttackSpeechComplete) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game->GetHero(),"TEXT_QST_038_BRIAR_SNIFFY",GROUP_SELECT_RANDOM_NO_REPEAT,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            } else if (!ParentClass.DoneBriarIntroSpeech) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                ParentClass.CutsceneStarted=true;
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectScriptedThingBase seh_hero;
                Game->StartScriptingEntity(Game->GetHero(),seh_hero,SCRIPT_AI_PRIORITY_HIGHEST);
                std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
                mymap["ROSE"]=seh_me;
                mymap["HERO"]=seh_hero;
                Game->FixMovieSequenceCamera(true);
                RunCutsceneMacro("CS_TROPHIES_INTRO",mymap,0,0,false,true);
                Game->FixMovieSequenceCamera(false);
                ParentClass.CutsceneStarted=false;
                ParentClass.DoneBriarIntroSpeech=true;
            } else if (!ParentClass.DoneBriarTrophySpeech) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                ParentClass.CutsceneStarted=true;
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectScriptedThingBase seh_hero;
                Game->StartScriptingEntity(Game->GetHero(),seh_hero,SCRIPT_AI_PRIORITY_HIGHEST);
                std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
                mymap["ROSE"]=seh_me;
                mymap["HERO"]=seh_hero;
                Game->FixMovieSequenceCamera(true);
                RunCutsceneMacro("CS_TROPHIES_TROPHY",mymap,0,0,false,true);
                Game->FixMovieSequenceCamera(false);
                ParentClass.CutsceneStarted=false;
                ParentClass.DoneBriarTrophySpeech=true;
                Game->ClearThingHasInformation(Me);
                Game->MiniMapRemoveMarker(Me);
            } else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game->GetHero(),"TEXT_QST_038_BRIAR_BLATHER",GROUP_SELECT_RANDOM_NO_REPEAT,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
        } else if (ParentClass.AttackedByHero) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (!ParentClass.AttackSpeechComplete) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game->ClearThingHasInformation(Me);
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
                    seh_me.Speak(Game->GetHero(),"TEXT_QST_038_BRIAR_ATTACK",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                ParentClass.AttackSpeechComplete=true;
            }
        }
        if (OpinionDeedTimer.GetTime()<=0) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->EntityPostOpinionDeedToAll(Me,OPINION_DEED_TYPE_SHOW_TROPHY_GOOD);
            OpinionDeedTimer=pGlobalScriptDef->TrophiesOpinionDeedTimer;
        }
    }
}
