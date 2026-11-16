#pragma optimize("s",on)
// CTCEntityEvents::Alloc(CThing& thing) -- the registry factory: a new CTCEntityEvents for `thing` (operator new(0x10) 0x00BFEA1A, null-checked, then
//   the constructor 0x004E3CD7), returned as its CTCBase
// Leaf: ?Alloc@CTCEntityEvents@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E48F6, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x10 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCEntityEvents::CTCEntityEvents ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCEntityEvents@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCEntityEvents@@QAE@AAVCThing@@@Z (0x004E3CD7, landed). (Landed earlier as a generated free function CTCEntityEvents_Alloc(CThing*) returning CTCEntityEvents*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCEntityEvents : public CTCBase {
public:
    CTCEntityEvents(CThing& thing);                        // 0x004E3CD7
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x10 - 0xC];              // the class's own members (not modelled); sizeof(CTCEntityEvents) = 0x10
};

CTCBase* __fastcall CTCEntityEvents::Alloc(CThing& thing)
{
    return new CTCEntityEvents(thing);
}
