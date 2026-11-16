#pragma optimize("s",on)
// CTCDestroyThingOnActionFinish::Alloc(CThing& thing) -- the registry factory: a new CTCDestroyThingOnActionFinish for `thing` (operator new(0xc) 0x00BFEA1A, null-checked, then
//   the constructor 0x004D6379), returned as its CTCBase
// Leaf: ?Alloc@CTCDestroyThingOnActionFinish@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D950E, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0xc ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCDestroyThingOnActionFinish::CTCDestroyThingOnActionFinish ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCDestroyThingOnActionFinish@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCDestroyThingOnActionFinish@@QAE@AAVCThing@@@Z (0x004D6379, landed). (Landed earlier as a generated free function CTCDestroyThingOnActionFinish_Alloc(CThing*) returning CTCDestroyThingOnActionFinish*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCDestroyThingOnActionFinish : public CTCBase {
public:
    CTCDestroyThingOnActionFinish(CThing& thing);                        // 0x004D6379
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0xc - 0xC];              // the class's own members (not modelled); sizeof(CTCDestroyThingOnActionFinish) = 0xc
};

CTCBase* __fastcall CTCDestroyThingOnActionFinish::Alloc(CThing& thing)
{
    return new CTCDestroyThingOnActionFinish(thing);
}
