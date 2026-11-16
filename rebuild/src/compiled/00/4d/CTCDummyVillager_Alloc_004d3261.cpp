#pragma optimize("s",on)
// CTCDummyVillager::Alloc(CThing& thing) -- the registry factory: a new CTCDummyVillager for `thing` (operator new(0x50) 0x00BFEA1A, null-checked, then
//   the constructor 0x0072F0F0), returned as its CTCBase
// Leaf: ?Alloc@CTCDummyVillager@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D3261, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x50 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCDummyVillager::CTCDummyVillager ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCDummyVillager@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCDummyVillager@@QAE@AAVCThing@@@Z (0x0072F0F0, landed). (Landed earlier as a generated free function CTCDummyVillager_Alloc(CThing*) returning CTCDummyVillager*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCDummyVillager : public CTCBase {
public:
    CTCDummyVillager(CThing& thing);                        // 0x0072F0F0
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x50 - 0xC];              // the class's own members (not modelled); sizeof(CTCDummyVillager) = 0x50
};

CTCBase* __fastcall CTCDummyVillager::Alloc(CThing& thing)
{
    return new CTCDummyVillager(thing);
}
