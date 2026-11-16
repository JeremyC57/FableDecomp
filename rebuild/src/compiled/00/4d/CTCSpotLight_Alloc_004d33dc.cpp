#pragma optimize("s",on)
// CTCSpotLight::Alloc(CThing& thing) -- the registry factory: a new CTCSpotLight for `thing` (operator new(0x2c) 0x00BFEA1A, null-checked, then
//   the constructor 0x00731D40), returned as its CTCBase
// Leaf: ?Alloc@CTCSpotLight@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D33DC, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x2c ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCSpotLight::CTCSpotLight ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCSpotLight@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCSpotLight@@QAE@AAVCThing@@@Z (0x00731D40, landed). (Landed earlier as a generated free function CTCSpotLight_Alloc(CThing*) returning CTCSpotLight*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCSpotLight : public CTCBase {
public:
    CTCSpotLight(CThing& thing);                        // 0x00731D40
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x2c - 0xC];              // the class's own members (not modelled); sizeof(CTCSpotLight) = 0x2c
};

CTCBase* __fastcall CTCSpotLight::Alloc(CThing& thing)
{
    return new CTCSpotLight(thing);
}
