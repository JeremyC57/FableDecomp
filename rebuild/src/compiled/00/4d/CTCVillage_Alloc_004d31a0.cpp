#pragma optimize("s",on)
// CTCVillage::Alloc(CThing& thing) -- the registry factory: a new CTCVillage for `thing` (operator new(0xe8) 0x00BFEA1A, null-checked, then
//   the constructor 0x0065B400), returned as its CTCBase
// Leaf: ?Alloc@CTCVillage@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D31A0, oracle length 32 bytes.
// Retail bytes:  push esi ; push 0xe8 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCVillage::CTCVillage ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCVillage@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCVillage@@QAE@AAVCThing@@@Z (0x0065B400, landed). (Landed earlier as a generated free function CTCVillage_Alloc(CThing*) returning CTCVillage*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCVillage : public CTCBase {
public:
    CTCVillage(CThing& thing);                        // 0x0065B400
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0xe8 - 0xC];              // the class's own members (not modelled); sizeof(CTCVillage) = 0xe8
};

CTCBase* __fastcall CTCVillage::Alloc(CThing& thing)
{
    return new CTCVillage(thing);
}
