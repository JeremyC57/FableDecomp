#pragma optimize("s",on)
// CTCPhysicsControlled::Alloc(CThing& thing) -- the registry factory: a new CTCPhysicsControlled for `thing` (operator new(0xac) 0x00BFEA1A, null-checked, then
//   the constructor 0x00724F50), returned as its CTCBase
// Leaf: ?Alloc@CTCPhysicsControlled@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D29AE, oracle length 32 bytes.
// Retail bytes:  push esi ; push 0xac ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCPhysicsControlled::CTCPhysicsControlled ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCPhysicsControlled@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); its only retail reference is
//   CTCBase::RegisterTCs 0x004EE23F, whose landed row already binds this spelling here (reloc_bindings.tsv). The constructor is
//   ??0CTCPhysicsControlled@@QAE@AAVCThing@@@Z (Ego_r + FableWin name sets), 0x00724F50, landed in the same unit.
// Naming (NF456, r454b unit with 0x00724F50): re-authored on the shared engine/CTCBase.h as the 0x004D393C pattern, same bytes (CTCBase is the
//   first base, so the upcast needs no adjustment). Was the generated free function CTCPhysicsControlled_Alloc(CThing*) returning CTCPhysicsControlled*, calling the
//   constructor as (CThing*).
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCPhysicsControlled : public CTCBase {
public:
    CTCPhysicsControlled(CThing& thing);                    // 0x00724F50
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0xac - 0xC];              // the class's own members (not modelled); sizeof(CTCPhysicsControlled) = 0xac
};

CTCBase* __fastcall CTCPhysicsControlled::Alloc(CThing& thing)
{
    return new CTCPhysicsControlled(thing);
}
