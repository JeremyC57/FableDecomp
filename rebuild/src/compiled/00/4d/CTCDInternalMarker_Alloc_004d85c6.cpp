#pragma optimize("s",on)
// CTCDInternalMarker::Alloc(CThing& thing) -- the registry factory: a new CTCDInternalMarker for `thing` (operator new(0xc) 0x00BFEA1A, null-checked, then
//   the constructor 0x004D4260), returned as its CTCBase
// Leaf: ?Alloc@CTCDInternalMarker@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D85C6, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0xc ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCDInternalMarker::CTCDInternalMarker ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCDInternalMarker@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCDInternalMarker@@QAE@AAVCThing@@@Z (0x004D4260, landed). (Landed earlier as a generated free function CTCDInternalMarker_Alloc(CThing*) returning CTCDInternalMarker*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCDInternalMarker : public CTCBase {
public:
    CTCDInternalMarker(CThing& thing);                        // 0x004D4260
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0xc - 0xC];              // the class's own members (not modelled); sizeof(CTCDInternalMarker) = 0xc
};

CTCBase* __fastcall CTCDInternalMarker::Alloc(CThing& thing)
{
    return new CTCDInternalMarker(thing);
}
