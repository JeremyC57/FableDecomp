#pragma optimize("s",on)
// CTCCameraPointBuilding::Alloc(CThing& thing) -- the registry factory: a new CTCCameraPointBuilding for `thing` (operator new(0x44) 0x00BFEA1A, null-checked, then
//   the constructor 0x004D8193), returned as its CTCBase
// Leaf: ?Alloc@CTCCameraPointBuilding@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DAFE7, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x44 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCCameraPointBuilding::CTCCameraPointBuilding ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCCameraPointBuilding@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCCameraPointBuilding@@QAE@AAVCThing@@@Z (0x004D8193, landed). (Landed earlier as a generated free function CTCCameraPointBuilding_Alloc(CThing*) returning CTCCameraPointBuilding*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCCameraPointBuilding : public CTCBase {
public:
    CTCCameraPointBuilding(CThing& thing);                        // 0x004D8193
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x44 - 0xC];              // the class's own members (not modelled); sizeof(CTCCameraPointBuilding) = 0x44
};

CTCBase* __fastcall CTCCameraPointBuilding::Alloc(CThing& thing)
{
    return new CTCCameraPointBuilding(thing);
}
