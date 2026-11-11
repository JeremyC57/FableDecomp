// NScript::CV_IntroductionToTrophiesScript::CTrophiesIntroWatcher::Main() -- trophy-intro crowd watcher: take scripted control of Me (priority 2); while Briar Rose lives and is within the crowd wander distance, give Me up during the cutscene, answer a hero talk (unless the hero attacked) with a paused TEXT_QST_038_CROWD line, and inside the tavern turn Me to face Briar Rose when it has been off camera for the timer's three seconds
// Leaf: ?Main@CTrophiesIntroWatcher@CV_IntroductionToTrophiesScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E9CFD0, 1833 bytes (+ int3 padding), plain `ret`, no EH frame.
// Identity (the ledger said NScript::CTrophiesIntroWatcher): slot 1 of the entity vtable 0x012ED614 (0x012ED618 is the only
//   reference; slot 0 the landed CTrophiesIntroWatcher `scalar deleting destructor' 0x00E9CFA0, slot 2 its Init 0x00E9CF90,
//   slot 3 GetParentScript 0x00E9CF80), stored by the landed CTrophiesIntroWatcher::Alloc 0x00E9CEE0 (the "TrophiesIntroWatcher"
//   binding of CV_IntroductionToTrophiesScript::Main 0x00E9C4B0); Ego_r.pdb / FableWin.pdb
//   ?Main@CTrophiesIntroWatcher@CV_IntroductionToTrophiesScript@NScript@@EAEXXZ (the only Main of a CTrophiesIntroWatcher in
//   either name set), so the enclosing script class is NScript::CV_IntroductionToTrophiesScript.
// Callees: CCharString 0x0099EBF0 / ~ 0x0099EAE0, CScriptThing ~ 0x004AA840 (expanded at the first and the CutsceneStarted-wait
//   exits, ~CBaseClass 0x0099A2E0), CBaseObject 0x0099A380 / ~ 0x0099A430, operator delete 0x00BFE9BC, scripted-thing handle
//   IsValid 0x00CD23B9 / Release 0x00CD2770 / GetScriptThing 0x007E7490 / Speak 0x007E7390 / IsPerformingScriptTask 0x007E7450 /
//   dtor 0x007E74D0, movie dtor 0x006E7B80 and ~CCountedPointer<CScriptGameResourceObjectMovieBase> 0x006E7AB0,
//   CEntityScriptBase::IsActiveThreadTerminating 0x00F35B30, IsDistanceBetweenThingsUnder 0x00CBE2FF (__fastcall);
//   CGameScriptInterfaceBase slots 7, 8 (priority 2 / 4), 70 GetHero, 72 GetThingWithScriptName("BriarRose" 0x012D42EC), 89
//   SetTimer / 90 GetTimer (CTimer through pDSTGame 0x0143E8F8), 264 GetHealth, 370, 379, 421 IsCameraPosOnScreen, 475
//   EntitySetFacingAngleTowardsThing; CScriptThing slots 6 GetPos, 27 MsgIsTalkedToBy ("SCRIPT_NAME_HERO"), 75 IsAlive;
//   "TEXT_QST_038_CROWD" 0x012ED630 (GROUP_SELECT_RANDOM); pGlobalScriptDef 0x0143E90C TrophiesCrowdWanderDistanceMax +0x6C0
//   (engine/CScriptDef.h).
// Layout: Ego_r.pdb CTrophiesIntroWatcher (0x30, the Alloc size): ParentClass +0x14, PMasterData +0x18, timer +0x1C,
//   timerStarted +0x20, initPos +0x21 (C3DVector, 1-aligned); parent CV_IntroductionToTrophiesScript = Ego_r - 0x14 (the
//   landed Init 0x00E9C3B0 / ctor 0x00E9C360 models): AttackedByHero +0x4A, InsideTavern +0x55, CutsceneStarted +0x60. No
//   engine header exists for the script class (local model as its landed siblings).
// Model: shared headers for everything else (CEntityScriptBase, CScriptThing, the CBaseObject-family handle / movie,
//   CWideScreenMagicPauseEntities with the per-TU FORCEINLINE_CTOR opt-in, CTimer, CScriptDef). Row-local inline bodies after
//   the includes, as the headers ask: ~CScriptThing and IsAlive (`PImp.Data && PImp.Data->IsAlive()`, the inline loop test),
//   CTimer::GetTime, and CTimer::operator= as __forceinline (retail expands all three SetTimer sites; plain inline calls the
//   Ego_r out-of-line copy at two of them, attempt a 1814v1833); the movie PImp dtor declared out of line (retail calls
//   0x006E7AB0 at the loop-continue movie dtor), as CDF_Dragon::Main 0x00E6FC40.
// Shape: the closing `if (IsActiveThreadTerminating()) return;` after the loop is retail's `call; (no test)` at 0x00E9D6D8;
//   the in-tavern timer is `if (!onScreen && !timerStarted) {..start..} else if (timerStarted && onScreen) {..stop..}` then
//   `if (timerStarted && timer.GetTime() == 0)` (VC threads the false arms straight to the loop head).
#include "fable_compiler.h"
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/C3DVector.h"
#include "engine/CScriptThing.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CIsThingAlive.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/ETextGroupSelectionMethod.h"
#include "engine/CTimer.h"
#include "engine/fable_script_def_global.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST = 0, SCRIPT_AI_PRIORITY_LOW = 1, SCRIPT_AI_PRIORITY_MEDIUM = 2,
                         SCRIPT_AI_PRIORITY_HIGH = 3, SCRIPT_AI_PRIORITY_HIGHEST = 4 };
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();   // 0x006E7AB0 (out of line in retail)
inline CScriptThing::~CScriptThing() { }
inline bool CScriptThing::IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8
FABLE_FORCEINLINE CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
inline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
bool FABLE_FASTCALL IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_IntroductionToTrophiesScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    enum BriarRoseOpinion { BriarOpinionUnspecified = 0 };
    bool DoneBriarIntroSpeech;              // +0x48
    bool DoneBriarTrophySpeech;             // +0x49
    bool AttackedByHero;                    // +0x4a
    bool DoneApproachMessage1;              // +0x4b
    bool DoneApproachMessage2;              // +0x4c
    bool AttackSpeechComplete;              // +0x4d
    long HitByHeroCount;                    // +0x50
    bool HitWarningComplete;                // +0x54
    bool InsideTavern;                      // +0x55
    bool QuestComplete;                     // +0x56
    BriarRoseOpinion OpinionCurrent;        // +0x58
    BriarRoseOpinion OpinionRequired;       // +0x5c
    bool CutsceneStarted;                   // +0x60
    bool OpinionTimerReset;                 // +0x61
    bool OutsideTavernTimerReset;           // +0x62
    CTimer OpinionTimer;                    // +0x64
    CTimer OutsideTavernTimer;              // +0x68
    class CTrophiesIntroWatcher : public CEntityScriptBase {
    public:
        CV_IntroductionToTrophiesScript* ParentClass;   // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
        CTimer timer;                                   // +0x1c
        bool timerStarted;                              // +0x20
        C3DVector initPos;                              // +0x21
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
        virtual void Init();
    };
};
}

void NScript::CV_IntroductionToTrophiesScript::CTrophiesIntroWatcher::Main()
{
    CScriptThing briarRose = Game->GetThingWithScriptName("BriarRose");
    CScriptGameResourceObjectScriptedThingBase watcher;
    if (watcher.IsValid())
        watcher.Release();
    while (!Game->StartScriptingEntity(Me, watcher, SCRIPT_AI_PRIORITY_MEDIUM)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    while (briarRose.IsAlive() && IsDistanceBetweenThingsUnder(Me, briarRose, pGlobalScriptDef->TrophiesCrowdWanderDistanceMax)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (ParentClass->CutsceneStarted) {
            if (IsActiveThreadTerminating())
                return;
            watcher.Release();
            while (ParentClass->CutsceneStarted) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            if (watcher.IsValid())
                watcher.Release();
            while (!Game->StartScriptingEntity(Me, watcher, SCRIPT_AI_PRIORITY_MEDIUM)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO") && !ParentClass->AttackedByHero) {
            if (IsActiveThreadTerminating())
                return;
            if (watcher.IsValid())
                watcher.Release();
            while (!Game->StartScriptingEntity(Me, watcher, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            CScriptGameResourceObjectMovieBase movie;
            Game->StartMovieSequence("", movie);
            CWideScreenMagicPauseEntities pause(*Game);
            if (Game->GetHealth(watcher.GetScriptThing()) > 0.0f) {
                watcher.Speak(Game->GetHero(), "TEXT_QST_038_CROWD", GROUP_SELECT_RANDOM, false, true, false);
                while (watcher.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
                if (IsActiveThreadTerminating())
                    return;
            }
            if (watcher.IsValid())
                watcher.Release();
            while (!Game->StartScriptingEntity(Me, watcher, SCRIPT_AI_PRIORITY_MEDIUM)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
        } else if (ParentClass->InsideTavern) {
            if (IsActiveThreadTerminating())
                return;
            if (!Game->IsCameraPosOnScreen(Me.GetPos()) && !timerStarted) {
                if (IsActiveThreadTerminating())
                    return;
                timerStarted = true;
                timer = 3;
            } else if (timerStarted && Game->IsCameraPosOnScreen(Me.GetPos())) {
                if (IsActiveThreadTerminating())
                    return;
                timerStarted = false;
                timer = 0;
            }
            if (timerStarted && timer.GetTime() == 0) {
                if (IsActiveThreadTerminating())
                    return;
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
                Game->EntitySetFacingAngleTowardsThing(Me, briarRose, false);
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
                timerStarted = false;
                timer = 3;
            }
        }
    }
    if (IsActiveThreadTerminating())
        return;
}