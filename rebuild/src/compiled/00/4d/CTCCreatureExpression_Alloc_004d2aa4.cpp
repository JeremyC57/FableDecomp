#pragma optimize("s",on)
// CTCCreatureExpression::Alloc(CThing& thing) -- the registry factory: a new CTCCreatureExpression for `thing` (operator new(0x14) 0x00BFEA1A, null-checked, then
//   the constructor 0x00729360), returned as its CTCBase
// Leaf: ?Alloc@CTCCreatureExpression@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D2AA4, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x14 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCCreatureExpression::CTCCreatureExpression ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCCreatureExpression@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCCreatureExpression@@QAE@AAVCThing@@@Z (0x00729360, landed). (Landed earlier as a generated free function CTCCreatureExpression_Alloc(CThing*) returning CTCCreatureExpression*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCCreatureExpression : public CTCBase {
public:
    CTCCreatureExpression(CThing& thing);                        // 0x00729360
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x14 - 0xC];              // the class's own members (not modelled); sizeof(CTCCreatureExpression) = 0x14
};

CTCBase* __fastcall CTCCreatureExpression::Alloc(CThing& thing)
{
    return new CTCCreatureExpression(thing);
}
