#pragma optimize("s",on)
// CTCFollowed::Alloc(CThing& thing) -- the registry factory: a new CTCFollowed for `thing` (operator new(0x28) 0x00BFEA1A, null-checked, then
//   the constructor 0x006D4E10), returned as its CTCBase
// Leaf: ?Alloc@CTCFollowed@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D5245, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x28 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCFollowed::CTCFollowed ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCFollowed@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCFollowed@@QAE@AAVCThing@@@Z (0x006D4E10, landed). (Landed earlier as a generated free function CTCFollowed_Alloc(CThing*) returning CTCFollowed*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCFollowed : public CTCBase {
public:
    CTCFollowed(CThing& thing);                        // 0x006D4E10
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x28 - 0xC];              // the class's own members (not modelled); sizeof(CTCFollowed) = 0x28
};

CTCBase* __fastcall CTCFollowed::Alloc(CThing& thing)
{
    return new CTCFollowed(thing);
}
