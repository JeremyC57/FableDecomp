#pragma optimize("s",on)
// CTCVillageMember::Alloc(CThing& thing) -- the registry factory: a new CTCVillageMember for `thing` (operator new(0x20) 0x00BFEA1A, null-checked, then
//   the constructor 0x004DAEE9), returned as its CTCBase
// Leaf: ?Alloc@CTCVillageMember@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DC620, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x20 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCVillageMember::CTCVillageMember ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCVillageMember@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCVillageMember@@QAE@AAVCThing@@@Z (0x004DAEE9, landed). (Landed earlier as a generated free function CTCVillageMember_Alloc(CThing*) returning CTCVillageMember*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCVillageMember : public CTCBase {
public:
    CTCVillageMember(CThing& thing);                        // 0x004DAEE9
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x20 - 0xC];              // the class's own members (not modelled); sizeof(CTCVillageMember) = 0x20
};

CTCBase* __fastcall CTCVillageMember::Alloc(CThing& thing)
{
    return new CTCVillageMember(thing);
}
