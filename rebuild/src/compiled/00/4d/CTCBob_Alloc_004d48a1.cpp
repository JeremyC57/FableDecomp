#pragma optimize("s",on)
// CTCBob::Alloc(CThing& thing) -- the registry factory: a new CTCBob for `thing` (operator new(0x34) 0x00BFEA1A, null-checked, then
//   the constructor 0x00787750), returned as its CTCBase
// Leaf: ?Alloc@CTCBob@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D48A1, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x34 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCBob::CTCBob ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCBob@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); its only retail reference is
//   CTCBase::RegisterTCs 0x004EE23F, whose landed row already binds this spelling here (reloc_bindings.tsv). The constructor is
//   ??0CTCBob@@QAE@AAVCThing@@@Z (Ego_r + FableWin name sets), 0x00787750, landed in the same unit.
// Naming (NF456, r454b unit with 0x00787750): re-authored on the shared engine/CTCBase.h as the 0x004D393C pattern, same bytes (CTCBase is the
//   first base, so the upcast needs no adjustment). Was the generated free function CTCBob_Alloc(CThing*) returning CTCBob*, calling the
//   constructor as (CThing*).
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCBob : public CTCBase {
public:
    CTCBob(CThing& thing);                                  // 0x00787750
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x34 - 0xC];              // the class's own members (not modelled); sizeof(CTCBob) = 0x34
};

CTCBase* __fastcall CTCBob::Alloc(CThing& thing)
{
    return new CTCBob(thing);
}
