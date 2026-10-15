// CTCJackPillar::OnInitialActivate() -- unless the engine globals' slot-9 query says otherwise, the pillar starts disabled
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007319A0, 29 bytes (+3 int3). rtti-vtables: CTCJackPillar 0x01266E84 slot 6.
// Callees: virtual slot 9 (+0x24, returns bool; name unknown -- the landed CMap::IsCameraPassableAt 0x0081F460
//          calls the same slot) of the engine-globals object behind the pointer @ 0x013B86A0 (the landed
//          CTCBoastUI::Deactivate 0x0056A880 reaches the camera manager through it), CTCJackPillar::Enable(bool) @ 0x007317D0.
//
// Shape: `mov esi,ecx ; mov ecx,[g] ; mov eax,[ecx] ; call [eax+24h] ; test al,al ; jne ; push 0 ; mov ecx,esi ; call`
// -- the global pointer loaded straight into ecx for the virtual call, the void call kept as `call` (the callee pops
// one argument, the caller none, so no sibling jump). The component derives from engine/CTCBase.h (vptr + ParentThing;
// CTCJackPillar itself is 12 bytes in the PDB, i.e. CTCBase only).
#include "engine/CTCBase.h"

struct CEngineGlobals {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08();
    virtual bool QuerySlot9();                // slot 9 (+0x24), name unknown
};
extern CEngineGlobals* g_pEngineGlobals;      // 0x013B86A0

struct CTCJackPillar : CTCBase {
    void Enable(bool enable);                 // 0x007317D0
    void OnInitialActivate();
};

void CTCJackPillar::OnInitialActivate()
{
    if (!g_pEngineGlobals->QuerySlot9())
        Enable(false);
}