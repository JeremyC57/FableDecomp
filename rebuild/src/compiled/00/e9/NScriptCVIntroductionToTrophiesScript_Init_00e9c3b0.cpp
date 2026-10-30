// NScript::CV_IntroductionToTrophiesScript::Init() -- reset the Briar Rose trophies introduction: speech flags, hit count, opinions and both timers
// Leaf: ?Init@CV_IntroductionToTrophiesScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E9C3B0, 93 bytes, thiscall, plain `ret`.
// Identity (proven): slot 3 (Init) of the RTTI-less level-script vtable 0x012ED558, which its constructor 0x00E9C360 (and Alloc 0x00E9FB40) stores;
//   level-script slots are 0 dtor, 1 RegisterMain, 2 Main, 3 Init, 4 OnPersist (FableWin.pdb CScriptBase order, the landed
//   level Init / OnPersist rows). Ego_r.pdb has ?Init@CV_IntroductionToTrophiesScript@NScript@@UAEXXZ.
// Layout: Ego_r.pdb CV_IntroductionToTrophiesScript members, each at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
// Identity, second source: Ego_r.pdb CV_IntroductionToTrophiesScript is the only level script with CTimers at +0x78/+0x7c
//   after bools and two BriarRoseOpinion values; slot 2 (Main 0x00E9C4B0) binds "BriarRose" / "TrophiesIntroWatcher".
// Callees: CTimer::operator=(long) inline: g_pGameScriptInterface 0x0143E8F8 slot 89 SetTimer.
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8 (the global script interface, bound name)
// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
inline CTimer& CTimer::operator=(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); return *this; }
enum BriarRoseOpinion { };                              // Ego_r.pdb enum (value 0 unnamed here)
class CPersistContext;
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#include "engine/CLevelScriptBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_IntroductionToTrophiesScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    bool             DoneBriarIntroSpeech;              // +0x48
    bool             DoneBriarTrophySpeech;             // +0x49
    bool             AttackedByHero;                    // +0x4a
    bool             DoneApproachMessage1;              // +0x4b
    bool             DoneApproachMessage2;              // +0x4c
    bool             AttackSpeechComplete;              // +0x4d
    long             HitByHeroCount;                    // +0x50
    bool             HitWarningComplete;                // +0x54
    bool             InsideTavern;                      // +0x55
    bool             QuestComplete;                     // +0x56
    BriarRoseOpinion OpinionCurrent;                    // +0x58
    BriarRoseOpinion OpinionRequired;                   // +0x5c
    bool             CutsceneStarted;                   // +0x60
    bool             OpinionTimerReset;                 // +0x61
    bool             OutsideTavernTimerReset;           // +0x62
    CTimer           OpinionTimer;                      // +0x64
    CTimer           OutsideTavernTimer;                // +0x68
    virtual void Init();
};

void CV_IntroductionToTrophiesScript::Init()
{
    DoneBriarIntroSpeech = false;
    DoneBriarTrophySpeech = false;
    AttackedByHero = false;
    DoneApproachMessage1 = false;
    DoneApproachMessage2 = false;
    AttackSpeechComplete = false;
    HitByHeroCount = 0;
    HitWarningComplete = true;
    InsideTavern = false;
    CutsceneStarted = false;
    OpinionCurrent = (BriarRoseOpinion)0;
    OpinionRequired = (BriarRoseOpinion)0;
    OpinionTimerReset = false;
    OutsideTavernTimerReset = false;
    QuestComplete = false;
    OpinionTimer = 0;
    OutsideTavernTimer = 0;
}
}