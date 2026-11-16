#pragma optimize("s",on)
// CTCGift::Alloc(CThing& thing) -- the registry factory: a new CTCGift for `thing` (operator new(0xc) 0x00BFEA1A, null-checked, then
//   the constructor 0x004D416D), returned as its CTCBase
// Leaf: ?Alloc@CTCGift@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D850E, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0xc ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCGift::CTCGift ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCGift@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCGift@@QAE@AAVCThing@@@Z (0x004D416D, landed). (Landed earlier as a generated free function CTCGift_Alloc(CThing*) returning CTCGift*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCGift : public CTCBase {
public:
    CTCGift(CThing& thing);                        // 0x004D416D
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0xc - 0xC];              // the class's own members (not modelled); sizeof(CTCGift) = 0xc
};

CTCBase* __fastcall CTCGift::Alloc(CThing& thing)
{
    return new CTCGift(thing);
}
