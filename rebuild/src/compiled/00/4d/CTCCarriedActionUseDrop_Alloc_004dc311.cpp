#pragma optimize("s",on)
// CTCCarriedActionUseDrop::Alloc(CThing& thing) -- the registry factory: a new CTCCarriedActionUseDrop for `thing` (operator new(0x14) 0x00BFEA1A, null-checked, then
//   the constructor 0x004DACAD), returned as its CTCBase
// Leaf: ?Alloc@CTCCarriedActionUseDrop@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DC311, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x14 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCCarriedActionUseDrop::CTCCarriedActionUseDrop ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCCarriedActionUseDrop@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCCarriedActionUseDrop@@QAE@AAVCThing@@@Z (0x004DACAD, landed). (Landed earlier as a generated free function CTCCarriedActionUseDrop_Alloc(CThing*) returning CTCCarriedActionUseDrop*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCCarriedActionUseDrop : public CTCBase {
public:
    CTCCarriedActionUseDrop(CThing& thing);                        // 0x004DACAD
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x14 - 0xC];              // the class's own members (not modelled); sizeof(CTCCarriedActionUseDrop) = 0x14
};

CTCBase* __fastcall CTCCarriedActionUseDrop::Alloc(CThing& thing)
{
    return new CTCCarriedActionUseDrop(thing);
}
