#pragma optimize("s",on)
// CTCFlammable::Alloc(CThing& thing) -- the registry factory: a new CTCFlammable for `thing` (operator new(0x34) 0x00BFEA1A, null-checked, then
//   the constructor 0x00780820), returned as its CTCBase
// Leaf: ?Alloc@CTCFlammable@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D46A0, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x34 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCFlammable::CTCFlammable ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCFlammable@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCFlammable@@QAE@AAVCThing@@@Z (0x00780820, landed). (Landed earlier as a generated free function CTCFlammable_Alloc(CThing*) returning CTCFlammable*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCFlammable : public CTCBase {
public:
    CTCFlammable(CThing& thing);                        // 0x00780820
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x34 - 0xC];              // the class's own members (not modelled); sizeof(CTCFlammable) = 0x34
};

CTCBase* __fastcall CTCFlammable::Alloc(CThing& thing)
{
    return new CTCFlammable(thing);
}
