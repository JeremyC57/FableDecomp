#pragma optimize("s",on)
// CTCCarriedActionUseLamp::Alloc(CThing& thing) -- the registry factory: a new CTCCarriedActionUseLamp for `thing` (operator new(0x14) 0x00BFEA1A, null-checked, then
//   the constructor 0x004DAC82), returned as its CTCBase
// Leaf: ?Alloc@CTCCarriedActionUseLamp@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DC2D8, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x14 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCCarriedActionUseLamp::CTCCarriedActionUseLamp ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCCarriedActionUseLamp@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCCarriedActionUseLamp@@QAE@AAVCThing@@@Z (0x004DAC82, landed). (Landed earlier as a generated free function CTCCarriedActionUseLamp_Alloc(CThing*) returning CTCCarriedActionUseLamp*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCCarriedActionUseLamp : public CTCBase {
public:
    CTCCarriedActionUseLamp(CThing& thing);                        // 0x004DAC82
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x14 - 0xC];              // the class's own members (not modelled); sizeof(CTCCarriedActionUseLamp) = 0x14
};

CTCBase* __fastcall CTCCarriedActionUseLamp::Alloc(CThing& thing)
{
    return new CTCCarriedActionUseLamp(thing);
}
