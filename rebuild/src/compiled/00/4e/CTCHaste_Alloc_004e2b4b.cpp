#pragma optimize("s",on)
// CTCHaste::Alloc(CThing& thing) -- the registry factory: a new CTCHaste for `thing` (operator new(0x24) 0x00BFEA1A, null-checked, then
//   the constructor 0x004E0C61), returned as its CTCBase
// Leaf: ?Alloc@CTCHaste@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E2B4B, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x24 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCHaste::CTCHaste ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCHaste@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCHaste@@QAE@AAVCThing@@@Z (0x004E0C61, landed). (Landed earlier as a generated free function CTCHaste_Alloc(CThing*) returning CTCHaste*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCHaste : public CTCBase {
public:
    CTCHaste(CThing& thing);                        // 0x004E0C61
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x24 - 0xC];              // the class's own members (not modelled); sizeof(CTCHaste) = 0x24
};

CTCBase* __fastcall CTCHaste::Alloc(CThing& thing)
{
    return new CTCHaste(thing);
}
