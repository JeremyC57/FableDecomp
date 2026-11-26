#pragma optimize("s",on)
// CTCPhotojournalCapture::Alloc(CThing& thing) -- the registry factory: a new CTCPhotojournalCapture for `thing` (operator new(0x48) 0x00BFEA1A, null-checked, then
//   the constructor 0x0055D520), returned as its CTCBase
// Leaf: ?Alloc@CTCPhotojournalCapture@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D670A, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x48 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCPhotojournalCapture::CTCPhotojournalCapture ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCPhotojournalCapture@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); its only retail reference is
//   CTCBase::RegisterTCs 0x004EE23F, whose landed row already binds this spelling here (reloc_bindings.tsv). The constructor is
//   ??0CTCPhotojournalCapture@@QAE@AAVCThing@@@Z (Ego_r + FableWin name sets), 0x0055D520, landed in the same unit.
// Naming (NF456, r454b unit with 0x0055D520): re-authored on the shared engine/CTCBase.h as the 0x004D393C pattern, same bytes (CTCBase is the
//   first base, so the upcast needs no adjustment). Was the generated free function CTCPhotojournalCapture_Alloc(CThing*) returning CTCPhotojournalCapture*, calling the
//   constructor as (CThing*).
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCPhotojournalCapture : public CTCBase {
public:
    CTCPhotojournalCapture(CThing& thing);                  // 0x0055D520
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x48 - 0xC];              // the class's own members (not modelled); sizeof(CTCPhotojournalCapture) = 0x48
};

CTCBase* __fastcall CTCPhotojournalCapture::Alloc(CThing& thing)
{
    return new CTCPhotojournalCapture(thing);
}
