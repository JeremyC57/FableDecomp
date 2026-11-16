#pragma optimize("s",on)
// CTCHeroAttachableAppearanceModifiers::Alloc(CThing& thing) -- the registry factory: a new CTCHeroAttachableAppearanceModifiers for `thing` (operator new(0x48) 0x00BFEA1A, null-checked, then
//   the constructor 0x00706D10), returned as its CTCBase
// Leaf: ?Alloc@CTCHeroAttachableAppearanceModifiers@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D4518, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x48 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCHeroAttachableAppearanceModifiers::CTCHeroAttachableAppearanceModifiers ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCHeroAttachableAppearanceModifiers@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCHeroAttachableAppearanceModifiers@@QAE@AAVCThing@@@Z (0x00706D10, landed). (Landed earlier as a generated free function CTCHeroAttachableAppearanceModifiers_Alloc(CThing*) returning CTCHeroAttachableAppearanceModifiers*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCHeroAttachableAppearanceModifiers : public CTCBase {
public:
    CTCHeroAttachableAppearanceModifiers(CThing& thing);                        // 0x00706D10
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x48 - 0xC];              // the class's own members (not modelled); sizeof(CTCHeroAttachableAppearanceModifiers) = 0x48
};

CTCBase* __fastcall CTCHeroAttachableAppearanceModifiers::Alloc(CThing& thing)
{
    return new CTCHeroAttachableAppearanceModifiers(thing);
}
