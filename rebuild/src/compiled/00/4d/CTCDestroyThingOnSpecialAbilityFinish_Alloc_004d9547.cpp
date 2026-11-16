#pragma optimize("s",on)
// CTCDestroyThingOnSpecialAbilityFinish::Alloc(CThing& thing) -- the registry factory: a new CTCDestroyThingOnSpecialAbilityFinish for `thing` (operator new(0xc) 0x00BFEA1A, null-checked, then
//   the constructor 0x004D63BA), returned as its CTCBase
// Leaf: ?Alloc@CTCDestroyThingOnSpecialAbilityFinish@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D9547, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0xc ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCDestroyThingOnSpecialAbilityFinish::CTCDestroyThingOnSpecialAbilityFinish ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCDestroyThingOnSpecialAbilityFinish@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCDestroyThingOnSpecialAbilityFinish@@QAE@AAVCThing@@@Z (0x004D63BA, landed). (Landed earlier as a generated free function CTCDestroyThingOnSpecialAbilityFinish_Alloc(CThing*) returning CTCDestroyThingOnSpecialAbilityFinish*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCDestroyThingOnSpecialAbilityFinish : public CTCBase {
public:
    CTCDestroyThingOnSpecialAbilityFinish(CThing& thing);                        // 0x004D63BA
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0xc - 0xC];              // the class's own members (not modelled); sizeof(CTCDestroyThingOnSpecialAbilityFinish) = 0xc
};

CTCBase* __fastcall CTCDestroyThingOnSpecialAbilityFinish::Alloc(CThing& thing)
{
    return new CTCDestroyThingOnSpecialAbilityFinish(thing);
}
