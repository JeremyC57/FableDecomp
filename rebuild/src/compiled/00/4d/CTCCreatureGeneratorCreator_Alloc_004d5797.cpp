#pragma optimize("s",on)
// CTCCreatureGeneratorCreator::Alloc(CThing& thing) -- the registry factory: a new CTCCreatureGeneratorCreator for `thing` (operator new(0x28) 0x00BFEA1A, null-checked, then
//   the constructor 0x007C9720), returned as its CTCBase
// Leaf: ?Alloc@CTCCreatureGeneratorCreator@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D5797, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x28 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCCreatureGeneratorCreator::CTCCreatureGeneratorCreator ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCCreatureGeneratorCreator@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCCreatureGeneratorCreator@@QAE@AAVCThing@@@Z (0x007C9720, landed). (Landed earlier as a generated free function CTCCreatureGeneratorCreator_Alloc(CThing*) returning CTCCreatureGeneratorCreator*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCCreatureGeneratorCreator : public CTCBase {
public:
    CTCCreatureGeneratorCreator(CThing& thing);                        // 0x007C9720
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x28 - 0xC];              // the class's own members (not modelled); sizeof(CTCCreatureGeneratorCreator) = 0x28
};

CTCBase* __fastcall CTCCreatureGeneratorCreator::Alloc(CThing& thing)
{
    return new CTCCreatureGeneratorCreator(thing);
}
