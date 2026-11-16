#pragma optimize("s",on)
// CTCBed::Alloc(CThing& thing) -- the registry factory: a new CTCBed for `thing` (operator new(0x18) 0x00BFEA1A, null-checked, then
//   the constructor 0x00746820), returned as its CTCBase
// Leaf: ?Alloc@CTCBed@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D35E2, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x18 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCBed::CTCBed ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCBed@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCBed@@QAE@AAVCThing@@@Z (0x00746820, landed). (Landed earlier as a generated free function CTCBed_Alloc(CThing*) returning CTCBed*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCBed : public CTCBase {
public:
    CTCBed(CThing& thing);                        // 0x00746820
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x18 - 0xC];              // the class's own members (not modelled); sizeof(CTCBed) = 0x18
};

CTCBase* __fastcall CTCBed::Alloc(CThing& thing)
{
    return new CTCBed(thing);
}
