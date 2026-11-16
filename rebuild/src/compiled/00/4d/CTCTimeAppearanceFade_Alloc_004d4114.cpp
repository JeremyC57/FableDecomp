#pragma optimize("s",on)
// CTCTimeAppearanceFade::Alloc(CThing& thing) -- the registry factory: a new CTCTimeAppearanceFade for `thing` (operator new(0xc) 0x00BFEA1A, null-checked, then
//   the constructor 0x007785E0), returned as its CTCBase
// Leaf: ?Alloc@CTCTimeAppearanceFade@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D4114, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0xc ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCTimeAppearanceFade::CTCTimeAppearanceFade ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCTimeAppearanceFade@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCTimeAppearanceFade@@QAE@AAVCThing@@@Z (0x007785E0, landed). (Landed earlier as a generated free function CTCTimeAppearanceFade_Alloc(CThing*) returning CTCTimeAppearanceFade*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCTimeAppearanceFade : public CTCBase {
public:
    CTCTimeAppearanceFade(CThing& thing);                        // 0x007785E0
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0xc - 0xC];              // the class's own members (not modelled); sizeof(CTCTimeAppearanceFade) = 0xc
};

CTCBase* __fastcall CTCTimeAppearanceFade::Alloc(CThing& thing)
{
    return new CTCTimeAppearanceFade(thing);
}
