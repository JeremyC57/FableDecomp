#pragma optimize("s",on)
// CTCDecapitation::Alloc(CThing& thing) -- the registry factory: a new CTCDecapitation for `thing` (operator new(0x14) 0x00BFEA1A, null-checked, then
//   the constructor 0x004DB207), returned as its CTCBase
// Leaf: ?Alloc@CTCDecapitation@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DCB95, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x14 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCDecapitation::CTCDecapitation ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCDecapitation@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCDecapitation@@QAE@AAVCThing@@@Z (0x004DB207, landed). (Landed earlier as a generated free function CTCDecapitation_Alloc(CThing*) returning CTCDecapitation*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCDecapitation : public CTCBase {
public:
    CTCDecapitation(CThing& thing);                        // 0x004DB207
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x14 - 0xC];              // the class's own members (not modelled); sizeof(CTCDecapitation) = 0x14
};

CTCBase* __fastcall CTCDecapitation::Alloc(CThing& thing)
{
    return new CTCDecapitation(thing);
}
