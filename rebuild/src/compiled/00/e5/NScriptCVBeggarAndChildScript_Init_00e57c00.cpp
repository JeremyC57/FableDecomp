// NScript::CV_BeggarAndChildScript::Init() -- clear the beggar-and-child flags and start both timers at 5
// Leaf: ?Init@CV_BeggarAndChildScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E57C00, 86 bytes, thiscall, plain `ret`.
// Identity (proven): slot 3 (Init) of the RTTI-less level-script vtable 0x012E65E0, which its constructor 0x00E57BB0 (and Alloc 0x00E5D0B0) stores;
//   level-script slots are 0 dtor, 1 RegisterMain, 2 Main, 3 Init, 4 OnPersist (FableWin.pdb CScriptBase order, the landed
//   level Init / OnPersist rows). Ego_r.pdb has ?Init@CV_BeggarAndChildScript@NScript@@UAEXXZ.
// Layout: Ego_r.pdb CV_BeggarAndChildScript members, each at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
// Identity, second source: Ego_r.pdb CV_BeggarAndChildScript is the only level script with nine bools, a long, three
//   bools and two CTimers at +0x5c..+0x74; slot 2 (Main 0x00E57E60) binds "LookoutPointBeggar" / "BeggarBully".
// Callees: CTimer::operator=(long) inline: g_pGameScriptInterface 0x0143E8F8 slot 89 SetTimer.
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8 (the global script interface, bound name)
class CTimer {                                          // Ego_r.pdb: one long TimerIndex
public:
    long TimerIndex;
    CTimer& operator=(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); return *this; }
};
class CPersistContext;
class CScriptBase {                                     // retail 0x40 bytes (Ego_r 0x54: debug containers)
public:
    virtual ~CScriptBase();                             // slot 0
protected:
    virtual void RegisterMain();                        // slot 1
    virtual void Main();                                // slot 2
    virtual void Init();                                // slot 3
public:
    virtual void OnPersist(CPersistContext& context);   // slot 4
    bool IsActiveThreadTerminating();                   // 0x00CB7940
    unsigned char _body[0x3c];
};
template <class TData> class CLevelScriptBase : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    TData*                MasterScriptData;             // +0x44
};

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_BeggarAndChildScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    bool   BeggarHit;                                   // +0x48
    bool   BeggarLeft;                                  // +0x49
    bool   BullyLeft;                                   // +0x4a
    bool   BullyHit;                                    // +0x4b
    bool   TaughtBelch;                                 // +0x4c
    bool   TaughtSneer;                                 // +0x4d
    bool   TaughtThanks;                                // +0x4e
    bool   TaughtBattleCry;                             // +0x4f
    bool   BullyHasTaunted;                             // +0x50
    long   BelchedAtBeggar;                             // +0x54
    bool   BelchedAtBully;                              // +0x58
    bool   QuestCardGiven;                              // +0x59
    bool   ExpressionTutorialShown;                     // +0x5a
    CTimer TauntTimer;                                  // +0x5c
    CTimer BeggarReplyTimer;                            // +0x60
    virtual void Init();
};

void CV_BeggarAndChildScript::Init()
{
    BeggarHit = false;
    BeggarLeft = false;
    BullyLeft = false;
    BullyHit = false;
    TaughtBelch = false;
    TaughtSneer = false;
    TaughtThanks = false;
    TaughtBattleCry = false;
    BullyHasTaunted = false;
    BelchedAtBeggar = 0;
    BelchedAtBully = false;
    QuestCardGiven = false;
    ExpressionTutorialShown = false;
    TauntTimer = 5;
    BeggarReplyTimer = 5;
}
}