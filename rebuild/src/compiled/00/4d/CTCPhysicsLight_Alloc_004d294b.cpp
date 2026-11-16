#pragma optimize("s",on)
// CTCPhysicsLight::Alloc(CThing& thing) -- the registry factory: a new CTCPhysicsLight for `thing` (operator new(0x44) 0x00BFEA1A, null-checked, then
//   the constructor 0x00723400), returned as its CTCBase
// Leaf: ?Alloc@CTCPhysicsLight@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D294B, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x44 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCPhysicsLight::CTCPhysicsLight ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCPhysicsLight@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCPhysicsLight@@QAE@AAVCThing@@@Z (0x00723400, landed). (Landed earlier as a generated free function CTCPhysicsLight_Alloc(CThing*) returning CTCPhysicsLight*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCPhysicsLight : public CTCBase {
public:
    CTCPhysicsLight(CThing& thing);                        // 0x00723400
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x44 - 0xC];              // the class's own members (not modelled); sizeof(CTCPhysicsLight) = 0x44
};

CTCBase* __fastcall CTCPhysicsLight::Alloc(CThing& thing)
{
    return new CTCPhysicsLight(thing);
}
