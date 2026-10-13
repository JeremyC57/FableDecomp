// CTCPhysicsBase::SetThingStandingOn(CTCPhysicsBase*)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006AF6D0, oracle length 40 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  mov eax,[esp+4] ; test eax,eax ; mov ecx,[ecx+4] ; mov edx,[ecx] ; je N ; mov eax,[eax+4] ; mov [esp+4],eax ; jmp [edx+94h] ; N: mov dword ptr [esp+4],0 ; jmp [edx+94h]
// engine/CTCPhysicsBase.h: +0x04 is the owning CThing (retail-only member `sub`). Forwards to the
// owning thing's virtual slot 37 with the other component's owning thing (or null): VC7.1 turns
// both branches into argument-slot overwrite + tail jump through the vtable.
#include "engine/CTCPhysicsBase.h"

struct CThingCalls;
struct CThingVirtuals {
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3c();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4c();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5c();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6c();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7c();
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8c();
    virtual void v90();
    virtual void SetThingStandingOn(CThingVirtuals* thing);   // slot 37 (+0x94)
};

struct CTCPhysicsBase_Methods : CTCPhysicsBase {
    void SetThingStandingOn(CTCPhysicsBase_Methods* other);
};

void CTCPhysicsBase_Methods::SetThingStandingOn(CTCPhysicsBase_Methods* other)
{
    CThingVirtuals* thing = reinterpret_cast<CThingVirtuals*>(this->sub);
    if (other)
        thing->SetThingStandingOn(reinterpret_cast<CThingVirtuals*>(other->sub));
    else
        thing->SetThingStandingOn(0);
}
