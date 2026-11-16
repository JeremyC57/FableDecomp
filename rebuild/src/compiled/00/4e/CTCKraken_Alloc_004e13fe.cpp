#pragma optimize("s",on)
// CTCKraken::Alloc(CThing& thing) -- the registry factory: a new CTCKraken for `thing` (operator new(0x68) 0x00BFEA1A, null-checked, then
//   the constructor 0x004DEFA7), returned as its CTCBase
// Leaf: ?Alloc@CTCKraken@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E13FE, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x68 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCKraken::CTCKraken ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCKraken@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCKraken@@QAE@AAVCThing@@@Z (0x004DEFA7, landed). (Landed earlier as a generated free function CTCKraken_Alloc(CThing*) returning CTCKraken*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCKraken : public CTCBase {
public:
    CTCKraken(CThing& thing);                        // 0x004DEFA7
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x68 - 0xC];              // the class's own members (not modelled); sizeof(CTCKraken) = 0x68
};

CTCBase* __fastcall CTCKraken::Alloc(CThing& thing)
{
    return new CTCKraken(thing);
}
