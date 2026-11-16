#pragma optimize("s",on)
// CTCAISummon::Alloc(CThing& thing) -- the registry factory: a new CTCAISummon for `thing` (operator new(0x18) 0x00BFEA1A, null-checked, then
//   the constructor 0x004DF2D4), returned as its CTCBase
// Leaf: ?Alloc@CTCAISummon@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E181D, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x18 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCAISummon::CTCAISummon ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCAISummon@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCAISummon@@QAE@AAVCThing@@@Z (0x004DF2D4, landed). (Landed earlier as a generated free function CTCAISummon_Alloc(CThing*) returning CTCAISummon*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCAISummon : public CTCBase {
public:
    CTCAISummon(CThing& thing);                        // 0x004DF2D4
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x18 - 0xC];              // the class's own members (not modelled); sizeof(CTCAISummon) = 0x18
};

CTCBase* __fastcall CTCAISummon::Alloc(CThing& thing)
{
    return new CTCAISummon(thing);
}
