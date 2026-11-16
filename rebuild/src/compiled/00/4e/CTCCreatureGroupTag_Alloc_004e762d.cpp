#pragma optimize("s",on)
// CTCCreatureGroupTag::Alloc(CThing& thing) -- the registry factory: a new CTCCreatureGroupTag for `thing` (operator new(0x38) 0x00BFEA1A, null-checked, then
//   the constructor 0x004E5F18), returned as its CTCBase
// Leaf: ?Alloc@CTCCreatureGroupTag@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E762D, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x38 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCCreatureGroupTag::CTCCreatureGroupTag ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCCreatureGroupTag@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCCreatureGroupTag@@QAE@AAVCThing@@@Z (0x004E5F18, landed). (Landed earlier as a generated free function CTCCreatureGroupTag_Alloc(CThing*) returning CTCCreatureGroupTag*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCCreatureGroupTag : public CTCBase {
public:
    CTCCreatureGroupTag(CThing& thing);                        // 0x004E5F18
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x38 - 0xC];              // the class's own members (not modelled); sizeof(CTCCreatureGroupTag) = 0x38
};

CTCBase* __fastcall CTCCreatureGroupTag::Alloc(CThing& thing)
{
    return new CTCCreatureGroupTag(thing);
}
