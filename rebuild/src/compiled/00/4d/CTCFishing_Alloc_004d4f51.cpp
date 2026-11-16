#pragma optimize("s",on)
// CTCFishing::Alloc(CThing& thing) -- the registry factory: a new CTCFishing for `thing` (operator new(0x64) 0x00BFEA1A, null-checked, then
//   the constructor 0x007B0800), returned as its CTCBase
// Leaf: ?Alloc@CTCFishing@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D4F51, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x64 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCFishing::CTCFishing ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCFishing@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCFishing@@QAE@AAVCThing@@@Z (0x007B0800, landed). (Landed earlier as a generated free function CTCFishing_Alloc(CThing*) returning CTCFishing*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCFishing : public CTCBase {
public:
    CTCFishing(CThing& thing);                        // 0x007B0800
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x64 - 0xC];              // the class's own members (not modelled); sizeof(CTCFishing) = 0x64
};

CTCBase* __fastcall CTCFishing::Alloc(CThing& thing)
{
    return new CTCFishing(thing);
}
