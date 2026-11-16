#pragma optimize("s",on)
// CTCActionUseAcceptBoast::Alloc(CThing& thing) -- the registry factory: a new CTCActionUseAcceptBoast for `thing` (operator new(0x14) 0x00BFEA1A, null-checked, then
//   the constructor 0x004DAB6C), returned as its CTCBase
// Leaf: ?Alloc@CTCActionUseAcceptBoast@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DC1F4, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x14 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCActionUseAcceptBoast::CTCActionUseAcceptBoast ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCActionUseAcceptBoast@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCActionUseAcceptBoast@@QAE@AAVCThing@@@Z (0x004DAB6C, landed). (Landed earlier as a generated free function CTCActionUseAcceptBoast_Alloc(CThing*) returning CTCActionUseAcceptBoast*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCActionUseAcceptBoast : public CTCBase {
public:
    CTCActionUseAcceptBoast(CThing& thing);                        // 0x004DAB6C
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x14 - 0xC];              // the class's own members (not modelled); sizeof(CTCActionUseAcceptBoast) = 0x14
};

CTCBase* __fastcall CTCActionUseAcceptBoast::Alloc(CThing& thing)
{
    return new CTCActionUseAcceptBoast(thing);
}
