#pragma optimize("s",on)
// CTCDiscovery::Alloc(CThing& thing) -- the registry factory: a new CTCDiscovery for `thing` (operator new(0x18) 0x00BFEA1A, null-checked, then
//   the constructor 0x007509A0), returned as its CTCBase
// Leaf: ?Alloc@CTCDiscovery@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D3857, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x18 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCDiscovery::CTCDiscovery ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCDiscovery@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCDiscovery@@QAE@AAVCThing@@@Z (0x007509A0, landed). (Landed earlier as a generated free function CTCDiscovery_Alloc(CThing*) returning CTCDiscovery*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCDiscovery : public CTCBase {
public:
    CTCDiscovery(CThing& thing);                        // 0x007509A0
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x18 - 0xC];              // the class's own members (not modelled); sizeof(CTCDiscovery) = 0x18
};

CTCBase* __fastcall CTCDiscovery::Alloc(CThing& thing)
{
    return new CTCDiscovery(thing);
}
