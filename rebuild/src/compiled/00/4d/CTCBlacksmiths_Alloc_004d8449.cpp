#pragma optimize("s",on)
// CTCBlacksmiths::Alloc(CThing& thing) -- the registry factory: a new CTCBlacksmiths for `thing` (operator new(0xc) 0x00BFEA1A, null-checked, then
//   the constructor 0x004D40BF), returned as its CTCBase
// Leaf: ?Alloc@CTCBlacksmiths@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D8449, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0xc ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCBlacksmiths::CTCBlacksmiths ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCBlacksmiths@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCBlacksmiths@@QAE@AAVCThing@@@Z (0x004D40BF, landed). (Landed earlier as a generated free function CTCBlacksmiths_Alloc(CThing*) returning CTCBlacksmiths*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCBlacksmiths : public CTCBase {
public:
    CTCBlacksmiths(CThing& thing);                        // 0x004D40BF
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0xc - 0xC];              // the class's own members (not modelled); sizeof(CTCBlacksmiths) = 0xc
};

CTCBase* __fastcall CTCBlacksmiths::Alloc(CThing& thing)
{
    return new CTCBlacksmiths(thing);
}
