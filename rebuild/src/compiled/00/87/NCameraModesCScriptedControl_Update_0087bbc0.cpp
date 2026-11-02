// NCameraModes::CScriptedControl::Update() -- the scripted camera mode's per-frame update: forwards to the script module's Update
// Leaf: ?Update@CScriptedControl@NCameraModes@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0087BBC0, length 11 bytes; vtable 0x0125C58C slot 1 (the only reference).
// Name: Ego_r + FableWin ?Update@CScriptedControl@NCameraModes@@UAEXXZ. Ego_r donor order GetDefaultFOV / GetDefaultRollAngle / OnInit /
// Update / IsShortenOnOcclusion / IsStretchAroundHero / OnDie = retail 0x0087BA30 / 0x0087BA60 / 0x0087BA90 / 0x0087BBC0 / 0x0087BBD0 /
// 0x0087BBE0 / 0x0087BBF0 (vtable slots 18 / 19 / 21 / 1 / 6 / 8 / 22); Ego_r's Update is also a 0x10-byte body.
// Callee: CScriptModuleBase slot 1 (+4), which is _purecall (0x00BFEA08) in the module vtable 0x01255F74 = the pure Update().
// Layout: retail CScriptedControl as the landed ctor 0x0069DFE0 / GetDefaultFOV 0x0087BA30: PScriptModule (CCountedPointer, Data
// first) at +0xa4 (Ego_r +0xc8 less the STLport container shrink).
#include "engine/CCountedPointer.h"

namespace NCameraModes {
class CScriptModuleBase { public:                                   // vtable 0x01255F74
    virtual ~CScriptModuleBase();                            // slot 0
    virtual void Update() = 0;                               // slot 1 (_purecall)
};
class CBase { public:                                               // retail sizeof 0xa4
    virtual ~CBase();                                        // slot 0
    virtual void Update();                                   // slot 1
    unsigned char _body[0xa4 - 4];
};
class CScriptedControl : public CBase { public:                     // vtable 0x0125C58C
    CCountedPointer<CScriptModuleBase> PScriptModule;        // +0xa4 (PDB +0xc8)
    float                              ObservedThingMeshHeightOnInit;   // +0xac
    virtual void Update();
};

void CScriptedControl::Update()
{
    PScriptModule->Update();
}
}