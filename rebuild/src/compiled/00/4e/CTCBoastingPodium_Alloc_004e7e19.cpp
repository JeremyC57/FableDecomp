#pragma optimize("s",on)
// CTCBoastingPodium::Alloc(CThing& thing) -- the registry factory: a new CTCBoastingPodium for `thing` (operator new(0x2c) 0x00BFEA1A, null-checked, then
//   the constructor 0x004E7513), returned as its CTCBase
// Leaf: ?Alloc@CTCBoastingPodium@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E7E19, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x2c ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCBoastingPodium::CTCBoastingPodium ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCBoastingPodium@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCBoastingPodium@@QAE@AAVCThing@@@Z (0x004E7513, landed). (Landed earlier as a generated free function CTCBoastingPodium_Alloc(CThing*) returning CTCBoastingPodium*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCBoastingPodium : public CTCBase {
public:
    CTCBoastingPodium(CThing& thing);                        // 0x004E7513
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x2c - 0xC];              // the class's own members (not modelled); sizeof(CTCBoastingPodium) = 0x2c
};

CTCBase* __fastcall CTCBoastingPodium::Alloc(CThing& thing)
{
    return new CTCBoastingPodium(thing);
}
