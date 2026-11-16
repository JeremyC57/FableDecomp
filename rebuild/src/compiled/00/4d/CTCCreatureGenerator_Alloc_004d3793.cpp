#pragma optimize("s",on)
// CTCCreatureGenerator::Alloc(CThing& thing) -- the registry factory: a new CTCCreatureGenerator for `thing` (operator new(0xa8) 0x00BFEA1A, null-checked, then
//   the constructor 0x0074AA00), returned as its CTCBase
// Leaf: ?Alloc@CTCCreatureGenerator@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D3793, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0xa8 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCCreatureGenerator::CTCCreatureGenerator ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCCreatureGenerator@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCCreatureGenerator@@QAE@AAVCThing@@@Z (0x0074AA00, landed). (Landed earlier as a generated free function CTCCreatureGenerator_Alloc(CThing*) returning CTCCreatureGenerator*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCCreatureGenerator : public CTCBase {
public:
    CTCCreatureGenerator(CThing& thing);                        // 0x0074AA00
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0xa8 - 0xC];              // the class's own members (not modelled); sizeof(CTCCreatureGenerator) = 0xa8
};

CTCBase* __fastcall CTCCreatureGenerator::Alloc(CThing& thing)
{
    return new CTCCreatureGenerator(thing);
}
