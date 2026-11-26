#pragma optimize("s",on)
// CTCLook::Alloc(CThing& thing) -- the registry factory: a new CTCLook for `thing` (operator new(0xd0) 0x00BFEA1A, null-checked, then
//   the constructor 0x00753170), returned as its CTCBase
// Leaf: ?Alloc@CTCLook@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D38F3, oracle length 32 bytes.
// Retail bytes:  push esi ; push 0xd0 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCLook::CTCLook ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCLook@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); its only retail reference is
//   CTCBase::RegisterTCs 0x004EE23F, whose landed row already binds this spelling here (reloc_bindings.tsv). The constructor is
//   ??0CTCLook@@QAE@AAVCThing@@@Z (Ego_r + FableWin name sets), 0x00753170, landed in the same unit.
// Naming (NF456, r454b unit with 0x00753170): re-authored on the shared engine/CTCBase.h as the 0x004D393C pattern, same bytes (CTCBase is the
//   first base, so the upcast needs no adjustment). Was the generated free function CTCLook_Alloc(CThing*) returning CTCLook*, calling the
//   constructor as (CThing*).
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCLook : public CTCBase {
public:
    CTCLook(CThing& thing);                                 // 0x00753170
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0xd0 - 0xC];              // the class's own members (not modelled); sizeof(CTCLook) = 0xd0
};

CTCBase* __fastcall CTCLook::Alloc(CThing& thing)
{
    return new CTCLook(thing);
}
