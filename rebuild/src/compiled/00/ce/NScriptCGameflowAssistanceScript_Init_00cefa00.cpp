// NScript::CGameflowAssistanceScript::Init() -- zero the assistance counters (flourishes, combat-multiplier tracking, warnings, saved renown) with the highest combat multiplier level at 1
// Leaf: ?Init@CGameflowAssistanceScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CEFA00, 31 bytes, thiscall, plain `ret` (virtual, level-script vtable slot 3).
// Identity (proven): slot 3 of the RTTI-less level-script vtable 0x012C5DE0, which the landed NScript::CGameflowAssistanceScript::CGameflowAssistanceScript 0x00CEF9D0 (and the Alloc 0x00CF0640) stores;
//   the level-script slots are 0 dtor, 1 RegisterMain, 2 Main, 3 Init, 4 OnPersist (FableWin.pdb CScriptBase order; slot 1
//   and 2 of these vtables are the landed RegisterMain / Main rows, slot 4 of 0x012CBA58 the landed CQ_EndGameScript::OnPersist
//   0x00D29830). Ego_r.pdb has ?Init@CGameflowAssistanceScript@NScript@@UAEXXZ.
// Layout: Ego_r.pdb CGameflowAssistanceScript members, each at Ego_r offset - 0x14 (retail CScriptBase is 0x40, Ego_r 0x54).
// Shape: retail stores SaveWarningsGiven (+0x64) before SavedRenownLevel (+0x60): the source order.
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
class CGameflowAssistanceScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    long FlourishesOffered;                             // +0x48
    long FlourishesPerformed;                           // +0x4c
    long CombatMultHighestLevel;                        // +0x50
    long CombatMultFramesSinceChange;                   // +0x54
    long HealthWarningsGiven;                           // +0x58
    long WillEnergyWarningsGiven;                       // +0x5c
    long SavedRenownLevel;                              // +0x60
    long SaveWarningsGiven;                             // +0x64
    virtual void Init();
};

void CGameflowAssistanceScript::Init()
{
    FlourishesOffered = 0;
    FlourishesPerformed = 0;
    CombatMultHighestLevel = 1;
    CombatMultFramesSinceChange = 0;
    HealthWarningsGiven = 0;
    WillEnergyWarningsGiven = 0;
    SaveWarningsGiven = 0;
    SavedRenownLevel = 0;
}
}