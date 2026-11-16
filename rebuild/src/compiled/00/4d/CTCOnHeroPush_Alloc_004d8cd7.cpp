#pragma optimize("s",on)
// CTCOnHeroPush::Alloc(CThing& thing) -- the registry factory: a new CTCOnHeroPush for `thing` (operator new(0x20) 0x00BFEA1A, null-checked, then
//   the constructor 0x004D5341), returned as its CTCBase
// Leaf: ?Alloc@CTCOnHeroPush@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D8CD7, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x20 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCOnHeroPush::CTCOnHeroPush ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCOnHeroPush@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCOnHeroPush@@QAE@AAVCThing@@@Z (0x004D5341, landed). (Landed earlier as a generated free function CTCOnHeroPush_Alloc(CThing*) returning CTCOnHeroPush*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCOnHeroPush : public CTCBase {
public:
    CTCOnHeroPush(CThing& thing);                        // 0x004D5341
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x20 - 0xC];              // the class's own members (not modelled); sizeof(CTCOnHeroPush) = 0x20
};

CTCBase* __fastcall CTCOnHeroPush::Alloc(CThing& thing)
{
    return new CTCOnHeroPush(thing);
}
