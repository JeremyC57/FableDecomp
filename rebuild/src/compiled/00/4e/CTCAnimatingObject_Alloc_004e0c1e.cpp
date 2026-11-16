#pragma optimize("s",on)
// CTCAnimatingObject::Alloc(CThing& thing) -- the registry factory: a new CTCAnimatingObject for `thing` (operator new(0x18) 0x00BFEA1A, null-checked, then
//   the constructor 0x004DE839), returned as its CTCBase
// Leaf: ?Alloc@CTCAnimatingObject@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E0C1E, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x18 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCAnimatingObject::CTCAnimatingObject ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCAnimatingObject@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCAnimatingObject@@QAE@AAVCThing@@@Z (0x004DE839, landed). (Landed earlier as a generated free function CTCAnimatingObject_Alloc(CThing*) returning CTCAnimatingObject*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCAnimatingObject : public CTCBase {
public:
    CTCAnimatingObject(CThing& thing);                        // 0x004DE839
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x18 - 0xC];              // the class's own members (not modelled); sizeof(CTCAnimatingObject) = 0x18
};

CTCBase* __fastcall CTCAnimatingObject::Alloc(CThing& thing)
{
    return new CTCAnimatingObject(thing);
}
