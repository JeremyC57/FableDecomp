// NScript::CV_IntroductionToTrophiesScript::CBriarRose::Main() -- Briar Rose in the tavern (Introduction to Trophies): face the M_TrophiesLookAtMe marker, then each frame switch her opinion-source loudness to the required level, warn the hero once he has hit her too often, play a random idle animation when her anim timer runs out, let a tavern villager comment as the hero approaches, answer when talked to (sniffy after an attack, the CS_TROPHIES_INTRO / CS_TROPHIES_TROPHY cut scenes, then blather), give the attack speech after the first attack, and post the trophy-show opinion deed on a timer
// Inline-budget: header-contract inline devices and two header opt-ins, no flag and no pragma (ablation/, each removed in turn from this source) -- FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR (retail expands the pause ctor in both talk blocks; without it DIFFER(5859v5915)); FABLE_RETAIL_STLP_MAP (the STLport map bodies retail compiled for the cast map: ctor / operator[] / dtor COMDATs; without it DIFFER(5947v5915)); row-local `inline CTimer::CTimer()` / `inline CTimer::~CTimer()` (retail expands RegisterTimer / DeregisterTimer at the timers' sites and calls ~CTimer 0x00CD4470 at the shared pads; without them DIFFER(5804v5915) / DIFFER(5656v5915)); row-local `FABLE_FORCEINLINE CTimer::operator=(long)` (the deed and anim timer stores expand at every site; plain inline DIFFER(5901v5915): the AnimTimer site is called; dropped DIFFER(5811v5915)); row-local `inline CTimer::GetTime()` (dropped DIFFER(5842v5915); FABLE_FORCEINLINE gives the same bytes, so the plain form); row-local `inline CScriptThing::CScriptThing()` / `~CScriptThing()` / `IsAlive()` (retail expands the ctor and IsAlive and splits the dtor between expansion and 0x004AA840; without them DIFFER(5881v5915) / DIFFER(5769v5915) / DIFFER(5925v5915)); `inline CIsThingAlive` ctor (the landed DemonDoors / Witch Mains' contract body; without it DIFFER(5891v5915)). The draft's movie ctor / dtor and handle-assign OUTOFLINE opt-ins and its two map out-of-line declarations change no byte and are dropped.
// Leaf: ?Main@CBriarRose@CV_IntroductionToTrophiesScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E9E420, 5915 bytes, thiscall, plain `ret`, no EH frame.
// Identity (proven, two sources): the address appears once in .rdata, at slot 1 of the entity vtable 0x012ED67C stored by
//   CBriarRose::Alloc 0x00E9D9D0 (slot 0 the landed scalar deleting dtor 0x00E9DA90, slot 2 the landed Init 0x00E9DAC0, slot 3 the
//   landed GetParentScript 0x00E9DA70); Ego_r.pdb and FableWin.pdb both hold ?Main@CBriarRose@CV_IntroductionToTrophiesScript@NScript@@EAEXXZ.
// Callees: CTimer (pDSTGame Register / Deregister / Set / GetTimer, ~CTimer 0x00CD4470), CScriptThing ctor (inline) / copy 0x004ABE90 /
//   ~ 0x004AA840 / IsAlive 0x004AB130, SetPredicate 0x00F35B10 with the CIsThingAlive temporary (dtor 0x00E241B0), IsActiveThreadTerminating
//   0x00F35B30, the scripted-thing handle (CBaseObject 0x0099A380, ctor 0x007E72A0, IsValid 0x00CD23B9, Release 0x00CD2770,
//   ClearCommands 0x007E7360, PlayAnimation 0x007E73D0, PlayLoopingAnimation 0x007E73F0, Speak 0x007E7390, IsPerformingScriptTask
//   0x007E7450, GetScriptThing 0x007E7490, operator= 0x008ABD10, ~ 0x007E74D0), MovieBase ctor 0x006E7B60 / dtor 0x006E7B80, cast
//   map ctor 0x00CDBF70 / dtor 0x00CDBFB0 / operator[] 0x00CD3D2E, RunCutsceneMacro 0x00CBFB7D, IsDistanceBetweenThingsUnder
//   0x00CBE2FF, rand 0x00BFEB16, CCharString 0x0099EBF0 / ~ 0x0099EAE0; NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations
//   0x01375748; interface slots through Game.
// Data: engine/CV_IntroductionToTrophiesScript.h (in the repo); pGlobalScriptDef TrophiesOpinionDeedTimer +0x69c,
//   TrophiesBriarRoseProximityNear / Far +0x6a8 / +0x6ac, TrophiesBriarRoseAnimTime / Var +0x6c4 / +0x6c8,
//   TrophiesHitCountForBriarAttack +0x6d4.
// Source shape: the parked chunk 828 draft's statement order, with three changes the bytes need: the main loop is
//   `for (;!false;Game->NewScriptFrame()) { if (IsActiveThreadTerminating()) return ..; ... }` (the generated endless-loop form of the landed
//   Mains; `for (;;..)` halves c2's block frequency of every in-loop site and flips the inline split of ~CTimer / ~CScriptThing / the movie
//   dtor at the pads, DIFFER(5945v5915)); the anim timer is `AnimTimer=pGlobalScriptDef->TrophiesBriarRoseAnimTime+rand()%..AnimTimeVar;`
//   (retail loads Var before rand() and reloads pGlobalScriptDef after it; the other operand order CSEs both loads and recolours the
//   prologue, DIFFER(5893v5915)); and the fitted read below. Names from Ego_d.pdb's locals: function scope deedTimer, standStillTimer, sehMe,
//   waypoint, _Fail_Val, LookAtMe; blocks {speech_movie, PauseMyEntitiesPlease}, {animIdx}, {villager, {HLStyleConversationIndex}} x2,
//   {speech_movie, PauseMyEntitiesPlease, {seh_hero, mymap} x2}, {speech_movie, PauseMyEntitiesPlease}. Ego_d (a later revision) also has
//   `counter`, `initialise` and {position_name}, which retail's code has no use for. Declaration order deedTimer, waypoint, standStillTimer
//   (retail's frame and prologue; Ego_d's frame offsets are a later layout).
// Inferred: redundant second read -- (fitted statement; coordinator ruling 2026-11-20 under the owner's delegation, on the 0x00E97070 terms; the
//   evidence is a register colour plus a hoist) the blather arm's first terminate check reads ParentClass.DoneBriarTrophySpeech a second time:
//   `if (ParentClass.DoneBriarTrophySpeech==true ? IsActiveThreadTerminating() : IsActiveThreadTerminating()) return ..;`. Retail tests the
//   trophy flag in al (`mov al,[eax+49h]; test al,al` 0x00E9F3FE) and hoists `mov ecx,esi` above the intro test's jne (0x00E9F234). A flag
//   read once is a code-generation temporary and takes cl (eax holds the ParentClass pointer at the load); a flag read twice with no call
//   between is a global candidate and gets al, which frees the hoist. The two identical arms are cross-jumped, so the second test leaves no
//   code. The bytes fix the read's position (before the blather arm's first call) and the merge, not its spelling: `>0` gives the same bytes;
//   the bare `ParentClass.DoneBriarTrophySpeech ? ..` is folded by c2 (the flag is known non-zero there) and grades DIFFER(5923v5915), so
//   `==true`, the generated scripts' comparison (landed 0x00ECF7F0, 0x00D58490). Retail-only, not in Ego_d: Ego_d's later revision (141 line
//   records, statements merged on macro lines) has no terminate-check-sized line with the /Od surplus this read costs (+47 bytes over the 87
//   of a plain check at VC7.1 /Od). Without it DIFFER(5923v5915), 187 diff lines.
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STLP_MAP
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
FABLE_FORCEINLINE CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
inline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
inline CScriptThing::CScriptThing() {}
inline CScriptThing::~CScriptThing() {}
inline bool CScriptThing::IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
void NScript::CV_IntroductionToTrophiesScript::CBriarRose::Main()
{
    TRetVoid _Fail_Val;
    CTimer deedTimer;
    CScriptThing waypoint;
    CTimer standStillTimer;
    AnimTimer=0;
    CScriptThing LookAtMe=Game->GetThingWithScriptName("M_TrophiesLookAtMe");
    if (LookAtMe.IsAlive()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->EntitySetFacingAngleTowardsThing(Me,LookAtMe,false);
    }
    deedTimer=pGlobalScriptDef->TrophiesOpinionDeedTimer;
    SetPredicate(CIsThingAlive(Me));
    CScriptGameResourceObjectScriptedThingBase sehMe;
    if (sehMe.IsValid()) sehMe.Release();
    while (!Game->StartScriptingEntity(Me,sehMe,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    for (;!false;Game->NewScriptFrame()) {
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
            if (sehMe.IsValid()) sehMe.Release();
            while (!Game->StartScriptingEntity(Me,sehMe,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (Game->GetHealth(sehMe.GetScriptThing())>0.0f) {
                sehMe.Speak(Game->GetHero(),"TEXT_QST_038_BRIAR_WARNING",GROUP_SELECT_RANDOM_NO_REPEAT,false,true,false);
                while (sehMe.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            ParentClass.HitWarningComplete=true;
        }
        if (AnimTimer.GetTime()==0 && sehMe.IsValid()) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            sehMe.ClearCommands();
            if (LookAtMe.IsAlive()) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game->EntitySetFacingAngleTowardsThing(Me,LookAtMe,false);
            }
            while (sehMe.IsPerformingScriptTask()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            long animIdx=rand()%8;
            if (animIdx==0) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                sehMe.PlayAnimation("ST_IDLE",false,false,true,false,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
            } else if (animIdx==1) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                sehMe.PlayAnimation("ST_IDLE_SUBTLE",false,false,true,false,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
            } else if (animIdx==2) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                sehMe.PlayAnimation("SCRIPT_TALK_IDLE_01",false,false,true,false,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
            } else if (animIdx==3) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                sehMe.PlayAnimation("SCRIPT_TALK_IDLE_02",false,false,true,false,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
            } else if (animIdx==4) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                sehMe.PlayAnimation("SCRIPT_TALK_IDLE_03",false,false,true,false,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
            } else if (animIdx==5) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            } else if (animIdx==6) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                sehMe.PlayAnimation("SCRIPT_LISTEN_BASE_IN",false,false,true,false,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
                sehMe.PlayLoopingAnimation("SCRIPT_LISTEN_BASE_LOOP",1,false,false,true,false,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
                sehMe.PlayAnimation("SCRIPT_LISTEN_BASE_OUT",false,false,true,true,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
            } else if (animIdx==7) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                sehMe.PlayAnimation("SCRIPT_FOLD_ARMS_IN",false,false,true,false,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
                sehMe.PlayLoopingAnimation("SCRIPT_FOLD_ARMS_LOOP",1,false,false,true,false,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
                sehMe.PlayAnimation("SCRIPT_FOLD_ARMS_OUT",false,false,true,true,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
            }
            AnimTimer=pGlobalScriptDef->TrophiesBriarRoseAnimTime+rand()%pGlobalScriptDef->TrophiesBriarRoseAnimTimeVar;
        }
        if (Game->GetHero().IsAlive() && ParentClass.InsideTavern) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (!ParentClass.DoneApproachMessage1 && IsDistanceBetweenThingsUnder(Me,Game->GetHero(),pGlobalScriptDef->TrophiesBriarRoseProximityFar)) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptThing villager=Game->GetRandomThingWithScriptName("TrophiesIntroWatcher");
                if (villager.IsAlive()) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    long HLStyleConversationIndex=Game->AddNewConversation(villager,false,false);
                    Game->AddPersonToConversation(HLStyleConversationIndex,Game->GetHero());
                    Game->AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_038_BRIAR_APPROACH_01",false,villager,Game->GetHero());
                }
                ParentClass.DoneApproachMessage1=true;
            }
            if (!ParentClass.DoneApproachMessage2 && IsDistanceBetweenThingsUnder(Me,Game->GetHero(),pGlobalScriptDef->TrophiesBriarRoseProximityNear)) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptThing villager=Game->GetRandomThingWithScriptName("TrophiesIntroWatcher");
                if (villager.IsAlive()) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    long HLStyleConversationIndex=Game->AddNewConversation(villager,false,false);
                    Game->AddPersonToConversation(HLStyleConversationIndex,Game->GetHero());
                    Game->AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_038_BRIAR_APPROACH_02",false,villager,Game->GetHero());
                }
                ParentClass.DoneApproachMessage2=true;
            }
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            sehMe.ClearCommands();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            while (sehMe.IsPerformingScriptTask()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (ParentClass.AttackedByHero && ParentClass.AttackSpeechComplete) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (sehMe.IsValid()) sehMe.Release();
                while (!Game->StartScriptingEntity(Me,sehMe,SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game->GetHealth(sehMe.GetScriptThing())>0.0f) {
                    sehMe.Speak(Game->GetHero(),"TEXT_QST_038_BRIAR_SNIFFY",GROUP_SELECT_RANDOM_NO_REPEAT,false,true,false);
                    while (sehMe.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            } else if (!ParentClass.DoneBriarIntroSpeech) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                ParentClass.CutsceneStarted=true;
                if (sehMe.IsValid()) sehMe.Release();
                while (!Game->StartScriptingEntity(Me,sehMe,SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectScriptedThingBase seh_hero;
                Game->StartScriptingEntity(Game->GetHero(),seh_hero,SCRIPT_AI_PRIORITY_HIGHEST);
                std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
                mymap["ROSE"]=sehMe;
                mymap["HERO"]=seh_hero;
                Game->FixMovieSequenceCamera(true);
                RunCutsceneMacro("CS_TROPHIES_INTRO",mymap,0,0,false,true);
                Game->FixMovieSequenceCamera(false);
                ParentClass.CutsceneStarted=false;
                ParentClass.DoneBriarIntroSpeech=true;
            } else if (!ParentClass.DoneBriarTrophySpeech) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                ParentClass.CutsceneStarted=true;
                if (sehMe.IsValid()) sehMe.Release();
                while (!Game->StartScriptingEntity(Me,sehMe,SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectScriptedThingBase seh_hero;
                Game->StartScriptingEntity(Game->GetHero(),seh_hero,SCRIPT_AI_PRIORITY_HIGHEST);
                std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
                mymap["ROSE"]=sehMe;
                mymap["HERO"]=seh_hero;
                Game->FixMovieSequenceCamera(true);
                RunCutsceneMacro("CS_TROPHIES_TROPHY",mymap,0,0,false,true);
                Game->FixMovieSequenceCamera(false);
                ParentClass.CutsceneStarted=false;
                ParentClass.DoneBriarTrophySpeech=true;
                Game->ClearThingHasInformation(Me);
                Game->MiniMapRemoveMarker(Me);
            } else {
                if (ParentClass.DoneBriarTrophySpeech==true ? IsActiveThreadTerminating() : IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (sehMe.IsValid()) sehMe.Release();
                while (!Game->StartScriptingEntity(Me,sehMe,SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game->GetHealth(sehMe.GetScriptThing())>0.0f) {
                    sehMe.Speak(Game->GetHero(),"TEXT_QST_038_BRIAR_BLATHER",GROUP_SELECT_RANDOM_NO_REPEAT,false,true,false);
                    while (sehMe.IsPerformingScriptTask()) {
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
                if (sehMe.IsValid()) sehMe.Release();
                while (!Game->StartScriptingEntity(Me,sehMe,SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (Game->GetHealth(sehMe.GetScriptThing())>0.0f) {
                    sehMe.Speak(Game->GetHero(),"TEXT_QST_038_BRIAR_ATTACK",GROUP_SELECT_NONE,false,true,false);
                    while (sehMe.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                ParentClass.AttackSpeechComplete=true;
            }
        }
        if (deedTimer.GetTime()<=0) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->EntityPostOpinionDeedToAll(Me,OPINION_DEED_TYPE_SHOW_TROPHY_GOOD);
            deedTimer=pGlobalScriptDef->TrophiesOpinionDeedTimer;
        }
    }
}