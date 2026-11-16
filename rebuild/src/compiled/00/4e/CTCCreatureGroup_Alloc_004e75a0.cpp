#pragma optimize("s",on)
// CTCCreatureGroup::Alloc(CThing& thing) -- the registry factory: a new CTCCreatureGroup for `thing` (operator new(0x24) 0x00BFEA1A, null-checked, then
//   the constructor 0x004E5C2F), returned as its CTCBase
// Leaf: ?Alloc@CTCCreatureGroup@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E75A0, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x24 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCCreatureGroup::CTCCreatureGroup ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCCreatureGroup@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCCreatureGroup@@QAE@AAVCThing@@@Z (0x004E5C2F, landed). (Landed earlier as a generated free function CTCCreatureGroup_Alloc(CThing*) returning CTCCreatureGroup*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCCreatureGroup : public CTCBase {
public:
    CTCCreatureGroup(CThing& thing);                        // 0x004E5C2F
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x24 - 0xC];              // the class's own members (not modelled); sizeof(CTCCreatureGroup) = 0x24
};

CTCBase* __fastcall CTCCreatureGroup::Alloc(CThing& thing)
{
    return new CTCCreatureGroup(thing);
}
