#pragma optimize("s",on)
// CTCActionUseBed::Alloc(CThing& thing) -- the registry factory: a new CTCActionUseBed for `thing` (operator new(0x34) 0x00BFEA1A, null-checked, then
//   the constructor 0x004DABBC), returned as its CTCBase
// Leaf: ?Alloc@CTCActionUseBed@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DC266, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x34 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCActionUseBed::CTCActionUseBed ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb / FableWin.pdb ?Alloc@CTCActionUseBed@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCActionUseBed@@QAE@AAVCThing@@@Z (0x004DABBC, landed). (Landed earlier as a generated free function CTCActionUseBed_Alloc(CThing*) returning CTCActionUseBed*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCActionUseBed : public CTCBase {
public:
    CTCActionUseBed(CThing& thing);                        // 0x004DABBC
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x34 - 0xC];              // the class's own members (not modelled); sizeof(CTCActionUseBed) = 0x34
};

CTCBase* __fastcall CTCActionUseBed::Alloc(CThing& thing)
{
    return new CTCActionUseBed(thing);
}
