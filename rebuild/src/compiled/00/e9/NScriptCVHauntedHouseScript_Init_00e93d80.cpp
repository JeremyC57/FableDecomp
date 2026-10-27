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
class CScriptBase {                                     // retail 0x40 bytes (Ego_r 0x54: debug containers)
public:
    virtual ~CScriptBase();                             // slot 0
protected:
    virtual void RegisterMain();                        // slot 1
    virtual void Main();                                // slot 2
    virtual void Init();                                // slot 3
public:
    virtual void OnPersist(CPersistContext& context);   // slot 4
    unsigned char _body[0x3c];
};
template <class TData> class CLevelScriptBase : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    TData*                MasterScriptData;             // +0x44
};

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