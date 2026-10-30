// NScript::CV_HauntedHouseScript::Init() -- reset the haunted house quest: bar, counters and flags, four undead at most
// Leaf: ?Init@CV_HauntedHouseScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E93D80, 31 bytes, thiscall, plain `ret` (virtual, level-script vtable slot 3).
// Identity (proven): slot 3 (Init) of the RTTI-less level-script vtable 0x012ECB94, which the landed NScript::CV_HauntedHouseScript::CV_HauntedHouseScript 0x00E93D50 (and the landed Alloc 0x00E96DD0) stores;
//   level-script slots are 0 dtor, 1 RegisterMain, 2 Main, 3 Init, 4 OnPersist (FableWin.pdb CScriptBase order, the landed
//   level Init / OnPersist rows). Ego_r.pdb has ?Init@CV_HauntedHouseScript@NScript@@UAEXXZ.
// Layout: Ego_r.pdb CV_HauntedHouseScript members, each at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
// Shape: the float zero is an integer store of eax.
class CGameScriptInterface;
class CPersistContext;
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
class CGameScriptInterfaceBase;

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_HauntedHouseScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    float UndeadBar;                                    // +0x48
    long  GuiCounter;                                   // +0x4c
    bool  CellarDoorOpen;                               // +0x50
    bool  HeroBeenToChest;                              // +0x51
    bool  QuestCardGiven;                               // +0x52
    bool  InHouse;                                      // +0x53
    long  UndeadAlive;                                  // +0x54
    long  MaxUndead;                                    // +0x58
    virtual void Init();
};

void CV_HauntedHouseScript::Init()
{
    UndeadBar = 0.0f;
    GuiCounter = 0;
    CellarDoorOpen = false;
    HeroBeenToChest = false;
    QuestCardGiven = false;
    InHouse = false;
    UndeadAlive = 0;
    MaxUndead = 4;
}
}