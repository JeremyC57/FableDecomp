// NScript::CV_HeroDuelScript::Init() -- reset the hero duel state (only OkToShowHealthBar starts true)
// Leaf: ?Init@CV_HeroDuelScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E986D0, 46 bytes, thiscall, plain `ret` (virtual, level-script vtable slot 3).
// Identity (proven): slot 3 (Init) of the RTTI-less level-script vtable 0x012ED0D4, which the landed NScript::CV_HeroDuelScript::CV_HeroDuelScript 0x00E986A0 (and the landed Alloc 0x00E9C300) stores;
//   level-script slots are 0 dtor, 1 RegisterMain, 2 Main, 3 Init, 4 OnPersist (FableWin.pdb CScriptBase order, the landed
//   level Init / OnPersist rows). Ego_r.pdb has ?Init@CV_HeroDuelScript@NScript@@UAEXXZ.
// Layout: Ego_r.pdb CV_HeroDuelScript members, each at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
class CGameScriptInterface;
class CPersistContext;
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
class CGameScriptInterfaceBase;

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_HeroDuelScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    bool          MissionSucceeded;                     // +0x48
    bool          MissionFailed;                        // +0x49
    bool          MissionOver;                          // +0x4a
    bool          GotToExecutionTree;                   // +0x4b
    bool          DuelStarted;                          // +0x4c
    bool          ThunderDefeated;                      // +0x4d
    bool          OkToShowHealthBar;                    // +0x4e
    bool          ThunderEnteredCaveFirst;              // +0x4f
    bool          OkToCheer;                            // +0x50
    bool          SupportThunder;                       // +0x51
    bool          StopSupportersCheering;               // +0x52
    bool          SaidSurprisedLine;                    // +0x53
    long          TimesHitSpectators;                   // +0x54
    unsigned long MyHealth;                             // +0x58
    virtual void Init();
};

void CV_HeroDuelScript::Init()
{
    MissionSucceeded = false;
    MissionFailed = false;
    MissionOver = false;
    GotToExecutionTree = false;
    DuelStarted = false;
    ThunderDefeated = false;
    OkToShowHealthBar = true;
    ThunderEnteredCaveFirst = false;
    OkToCheer = false;
    SupportThunder = false;
    StopSupportersCheering = false;
    SaidSurprisedLine = false;
    TimesHitSpectators = 0;
    MyHealth = 0;
}
}