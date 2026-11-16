#pragma optimize("s",on)
// CTCActionUsePickUpGenericBox::Alloc(CThing& thing) -- the registry factory: a new CTCActionUsePickUpGenericBox for `thing` (operator new(0x14) 0x00BFEA1A, null-checked, then
//   the constructor 0x004DAA7C), returned as its CTCBase
// Leaf: ?Alloc@CTCActionUsePickUpGenericBox@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DC09E, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x14 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCActionUsePickUpGenericBox::CTCActionUsePickUpGenericBox ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCActionUsePickUpGenericBox@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCActionUsePickUpGenericBox@@QAE@AAVCThing@@@Z (0x004DAA7C, landed). (Landed earlier as a generated free function CTCActionUsePickUpGenericBox_Alloc(CThing*) returning CTCActionUsePickUpGenericBox*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCActionUsePickUpGenericBox : public CTCBase {
public:
    CTCActionUsePickUpGenericBox(CThing& thing);                        // 0x004DAA7C
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x14 - 0xC];              // the class's own members (not modelled); sizeof(CTCActionUsePickUpGenericBox) = 0x14
};

CTCBase* __fastcall CTCActionUsePickUpGenericBox::Alloc(CThing& thing)
{
    return new CTCActionUsePickUpGenericBox(thing);
}
