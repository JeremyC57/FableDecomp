// CTCJackShield::vfunc_6() -- slot 6 (the initial-activate slot): disables the shield and, unless the engine globals' slot-9 query says otherwise, calls the owner thing's virtual slot 28 with false
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00731970, 37 bytes (+11 int3). rtti-vtables: CTCJackShield 0x01266EFC slot 6
// (CTCJackPillar's slot 6 is labelled OnInitialActivate; the catalog label "vfunc_6" is kept).
// Callees: CTCJackShield::EnableShield(bool) @ 0x00731680, virtual slot 9 (+0x24, bool; name unknown) of the engine globals
//          behind @ 0x013B86A0, virtual slot 28 (+0x70) of CTCBase::ParentThing (name unknown).
//
// Shape: `push esi ; push 0 ; mov esi,ecx ; call EnableShield` (the constant pushed before `this` is saved), then the
// global loaded into ecx for the slot-9 call, and on false `mov ecx,[esi+4] ; mov edx,[ecx] ; push 0 ; call [edx+70h]`.
// The component derives from engine/CTCBase.h (ParentThing at +4); CThing is modelled by its vtable only.
#include "engine/CTCBase.h"

struct CEngineGlobals {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08();
    virtual bool QuerySlot9();                // slot 9 (+0x24), name unknown
};
extern CEngineGlobals* g_pEngineGlobals;      // 0x013B86A0

struct CThing {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void Slot28(bool flag);           // slot 28 (+0x70), name unknown
};

struct CTCJackShield : CTCBase {
    void EnableShield(bool enable);           // 0x00731680
    void vfunc_6();
};

void CTCJackShield::vfunc_6()
{
    EnableShield(false);
    if (!g_pEngineGlobals->QuerySlot9())
        ParentThing->Slot28(false);
}
