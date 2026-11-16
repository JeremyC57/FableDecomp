#pragma optimize("s",on)
// CTCPhysicsRigidBody::Alloc(CThing& thing) -- the registry factory: a new CTCPhysicsRigidBody for `thing` (operator new(0x258) 0x00BFEA1A, null-checked, then
//   the constructor 0x0075AA80), returned as its CTCBase
// Leaf: ?Alloc@CTCPhysicsRigidBody@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D399C, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x258 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCPhysicsRigidBody::CTCPhysicsRigidBody ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCPhysicsRigidBody@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCPhysicsRigidBody@@QAE@AAVCThing@@@Z (0x0075AA80, landed). (Landed earlier as a generated free function CTCPhysicsRigidBody_Alloc(CThing*) returning CTCPhysicsRigidBody*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCPhysicsRigidBody : public CTCBase {
public:
    CTCPhysicsRigidBody(CThing& thing);                        // 0x0075AA80
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x258 - 0xC];              // the class's own members (not modelled); sizeof(CTCPhysicsRigidBody) = 0x258
};

CTCBase* __fastcall CTCPhysicsRigidBody::Alloc(CThing& thing)
{
    return new CTCPhysicsRigidBody(thing);
}
