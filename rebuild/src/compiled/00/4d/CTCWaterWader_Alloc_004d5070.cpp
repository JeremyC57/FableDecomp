#pragma optimize("s",on)
// CTCWaterWader::Alloc(CThing& thing) -- the registry factory: a new CTCWaterWader for `thing` (operator new(0x20) 0x00BFEA1A, null-checked, then
//   the constructor 0x007B5B40), returned as its CTCBase
// Leaf: ?Alloc@CTCWaterWader@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D5070, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x20 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCWaterWader::CTCWaterWader ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCWaterWader@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCWaterWader@@QAE@AAVCThing@@@Z (0x007B5B40, landed). (Landed earlier as a generated free function CTCWaterWader_Alloc(CThing*) returning CTCWaterWader*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCWaterWader : public CTCBase {
public:
    CTCWaterWader(CThing& thing);                        // 0x007B5B40
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x20 - 0xC];              // the class's own members (not modelled); sizeof(CTCWaterWader) = 0x20
};

CTCBase* __fastcall CTCWaterWader::Alloc(CThing& thing)
{
    return new CTCWaterWader(thing);
}
