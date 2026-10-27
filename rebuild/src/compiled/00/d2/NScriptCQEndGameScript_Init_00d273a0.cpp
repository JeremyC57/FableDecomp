// NScript::CQ_EndGameScript::Init() -- clear the end-game state: undead bar, GUI counter and the abbey / quest-start / mother / undead-bar flags
// Leaf: ?Init@CQ_EndGameScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D273A0, 24 bytes, thiscall, plain `ret` (virtual, level-script vtable slot 3).
// Identity (proven): slot 3 of the RTTI-less level-script vtable 0x012CBA58, which the landed NScript::CQ_EndGameScript::CQ_EndGameScript 0x00D27370 (and the Alloc 0x00D2A7C0) stores;
//   the level-script slots are 0 dtor, 1 RegisterMain, 2 Main, 3 Init, 4 OnPersist (FableWin.pdb CScriptBase order; slot 1
//   and 2 of these vtables are the landed RegisterMain / Main rows, slot 4 of 0x012CBA58 the landed CQ_EndGameScript::OnPersist
//   0x00D29830). Ego_r.pdb has ?Init@CQ_EndGameScript@NScript@@UAEXXZ.
// Layout: Ego_r.pdb CQ_EndGameScript members, each at Ego_r offset - 0x14.
// Shape: the float zero is an integer store of eax.
class CGameScriptInterfaceBase;
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
    CGameScriptInterfaceBase* Game;                     // +0x40
    TData*                    MasterScriptData;         // +0x44
};

namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_EndGameScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    float UndeadBar;                                    // +0x48
    long  GuiCounter;                                   // +0x4c
    bool  SeenAbbey;                                    // +0x50
    bool  QuestStartScreened;                           // +0x51
    bool  SeenMotherDisappear;                          // +0x52
    bool  MotherSpoken;                                 // +0x53
    bool  ShowUndeadBar;                                // +0x54
    virtual void Init();
};

void CQ_EndGameScript::Init()
{
    UndeadBar = 0.0f;
    GuiCounter = 0;
    SeenAbbey = false;
    QuestStartScreened = false;
    SeenMotherDisappear = false;
    MotherSpoken = false;
    ShowUndeadBar = false;
}
}