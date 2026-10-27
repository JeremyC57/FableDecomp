// NScript::CQ_HerosOldHouseScript::Init() -- reset the script's state: Helping, Helped, WifeAttacked, WifeAttackedAndLeft, BootyDugUp = 0/false
// Leaf: ?Init@CQ_HerosOldHouseScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D89DA0, 18 bytes, thiscall, plain `ret`.
// Identity (proven): slot 3 of the script vtable 0x012D5018 (stored by the landed script constructor / Alloc); FableWin.pdb
//   ?Init@CQ_HerosOldHouseScript@NScript@@UAEXXZ; the members it clears are Ego_r.pdb's CQ_HerosOldHouseScript fields at their PDB - 0x14 offsets.
// Shape (family K066, representative 0x00CF06D0): one zero register (al, or eax when a long is cleared) and one store per
//   member in source order; MissionAborted is left alone.
class CScriptDataBase;
class CGameScriptInterfaceBase;
class CPersistContext;
class CScriptBase {                                     // retail 0x40 bytes
public:
    virtual ~CScriptBase();
protected:
    virtual void RegisterMain();
    virtual void Main();
    virtual void Init();                                // slot 3
public:
    virtual void OnPersist(CPersistContext& context);
    unsigned char _body[0x3c];                          // +0x04 .. +0x40
};
class CLevelScriptBase : public CScriptBase {           // PDB CLevelScriptBase<TData> (TData enters no symbol here)
public:
    CGameScriptInterfaceBase* Game;                     // +0x40
    CScriptDataBase* MasterScriptData;                  // +0x44
};
namespace NScript {
class CQ_HerosOldHouseScript : public CLevelScriptBase {
public:
    virtual void Init();
    bool  Helping;                                 // +0x48
    bool  Helped;                                  // +0x49
    bool  MissionAborted;                          // +0x4A
    bool  WifeAttacked;                            // +0x4B
    bool  WifeAttackedAndLeft;                     // +0x4C
    bool  BootyDugUp;                              // +0x4D
};

void CQ_HerosOldHouseScript::Init()
{
    Helping = false;
    Helped = false;
    WifeAttacked = false;
    WifeAttackedAndLeft = false;
    BootyDugUp = false;
}
}