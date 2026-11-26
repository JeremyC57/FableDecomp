#pragma optimize("s",on)
// CTCStealth::Alloc(CThing& thing) -- the registry factory: a new CTCStealth for `thing` (operator new(0x4c) 0x00BFEA1A, null-checked, then
//   the constructor 0x00747820), returned as its CTCBase
// Leaf: ?Alloc@CTCStealth@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D3612, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x4c ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCStealth::CTCStealth ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCStealth@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); its only retail reference is
//   CTCBase::RegisterTCs 0x004EE23F, whose landed row already binds this spelling here (reloc_bindings.tsv). The constructor is
//   ??0CTCStealth@@QAE@AAVCThing@@@Z (Ego_r + FableWin name sets), 0x00747820, landed in the same unit.
// Naming (NF456, r454b unit with 0x00747820): re-authored on the shared engine/CTCBase.h as the 0x004D393C pattern, same bytes (CTCBase is the
//   first base, so the upcast needs no adjustment). Was the generated free function CTCStealth_Alloc(CThing*) returning CTCStealth*, calling the
//   constructor as (CThing*).
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCStealth : public CTCBase {
public:
    CTCStealth(CThing& thing);                              // 0x00747820
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x4c - 0xC];              // the class's own members (not modelled); sizeof(CTCStealth) = 0x4c
};

CTCBase* __fastcall CTCStealth::Alloc(CThing& thing)
{
    return new CTCStealth(thing);
}
