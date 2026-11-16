#pragma optimize("s",on)
// CTCCreatureItemLevitationHero::Alloc(CThing& thing) -- the registry factory: a new CTCCreatureItemLevitationHero for `thing` (operator new(0x18) 0x00BFEA1A, null-checked, then
//   the constructor 0x004DE742), returned as its CTCBase
// Leaf: ?Alloc@CTCCreatureItemLevitationHero@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E09C7, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x18 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCCreatureItemLevitationHero::CTCCreatureItemLevitationHero ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCCreatureItemLevitationHero@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCCreatureItemLevitationHero@@QAE@AAVCThing@@@Z (0x004DE742, landed). (Landed earlier as a generated free function CTCCreatureItemLevitationHero_Alloc(CThing*) returning CTCCreatureItemLevitationHero*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCCreatureItemLevitationHero : public CTCBase {
public:
    CTCCreatureItemLevitationHero(CThing& thing);                        // 0x004DE742
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x18 - 0xC];              // the class's own members (not modelled); sizeof(CTCCreatureItemLevitationHero) = 0x18
};

CTCBase* __fastcall CTCCreatureItemLevitationHero::Alloc(CThing& thing)
{
    return new CTCCreatureItemLevitationHero(thing);
}
