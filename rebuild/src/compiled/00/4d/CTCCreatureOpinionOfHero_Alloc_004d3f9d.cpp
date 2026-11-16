#pragma optimize("s",on)
// CTCCreatureOpinionOfHero::Alloc(CThing& thing) -- the registry factory: a new CTCCreatureOpinionOfHero for `thing` (operator new(0x350) 0x00BFEA1A, null-checked, then
//   the constructor 0x00684420), returned as its CTCBase
// Leaf: ?Alloc@CTCCreatureOpinionOfHero@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D3F9D, oracle length 32 bytes.
// Retail bytes:  push esi ; push 0x350 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCCreatureOpinionOfHero::CTCCreatureOpinionOfHero ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCCreatureOpinionOfHero@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCCreatureOpinionOfHero@@QAE@AAVCThing@@@Z (0x00684420, landed). (Landed earlier as a generated free function CTCCreatureOpinionOfHero_Alloc(CThing*) returning CTCCreatureOpinionOfHero*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCCreatureOpinionOfHero : public CTCBase {
public:
    CTCCreatureOpinionOfHero(CThing& thing);                        // 0x00684420
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x350 - 0xC];              // the class's own members (not modelled); sizeof(CTCCreatureOpinionOfHero) = 0x350
};

CTCBase* __fastcall CTCCreatureOpinionOfHero::Alloc(CThing& thing)
{
    return new CTCCreatureOpinionOfHero(thing);
}
