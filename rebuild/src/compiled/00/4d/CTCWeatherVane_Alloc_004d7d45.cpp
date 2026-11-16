#pragma optimize("s",on)
// CTCWeatherVane::Alloc(CThing& thing) -- the registry factory: a new CTCWeatherVane for `thing` (operator new(0x10) 0x00BFEA1A, null-checked, then
//   the constructor 0x004D34AB), returned as its CTCBase
// Leaf: ?Alloc@CTCWeatherVane@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D7D45, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x10 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCWeatherVane::CTCWeatherVane ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCWeatherVane@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCWeatherVane@@QAE@AAVCThing@@@Z (0x004D34AB, landed). (Landed earlier as a generated free function CTCWeatherVane_Alloc(CThing*) returning CTCWeatherVane*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCWeatherVane : public CTCBase {
public:
    CTCWeatherVane(CThing& thing);                        // 0x004D34AB
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x10 - 0xC];              // the class's own members (not modelled); sizeof(CTCWeatherVane) = 0x10
};

CTCBase* __fastcall CTCWeatherVane::Alloc(CThing& thing)
{
    return new CTCWeatherVane(thing);
}
