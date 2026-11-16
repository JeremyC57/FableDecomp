#pragma optimize("s",on)
// CTCElectrocutedEffect::Alloc(CThing& thing) -- the registry factory: a new CTCElectrocutedEffect for `thing` (operator new(0x34) 0x00BFEA1A, null-checked, then
//   the constructor 0x007CEB00), returned as its CTCBase
// Leaf: ?Alloc@CTCElectrocutedEffect@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D58F6, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x34 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCElectrocutedEffect::CTCElectrocutedEffect ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCElectrocutedEffect@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCElectrocutedEffect@@QAE@AAVCThing@@@Z (0x007CEB00, landed). (Landed earlier as a generated free function CTCElectrocutedEffect_Alloc(CThing*) returning CTCElectrocutedEffect*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCElectrocutedEffect : public CTCBase {
public:
    CTCElectrocutedEffect(CThing& thing);                        // 0x007CEB00
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x34 - 0xC];              // the class's own members (not modelled); sizeof(CTCElectrocutedEffect) = 0x34
};

CTCBase* __fastcall CTCElectrocutedEffect::Alloc(CThing& thing)
{
    return new CTCElectrocutedEffect(thing);
}
