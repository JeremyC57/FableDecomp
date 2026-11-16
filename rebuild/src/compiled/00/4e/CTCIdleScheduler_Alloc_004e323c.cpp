#pragma optimize("s",on)
// CTCIdleScheduler::Alloc(CThing& thing) -- the registry factory: a new CTCIdleScheduler for `thing` (operator new(0x2c) 0x00BFEA1A, null-checked, then
//   the constructor 0x004E1537), returned as its CTCBase
// Leaf: ?Alloc@CTCIdleScheduler@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E323C, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x2c ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCIdleScheduler::CTCIdleScheduler ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCIdleScheduler@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCIdleScheduler@@QAE@AAVCThing@@@Z (0x004E1537, landed). (Landed earlier as a generated free function CTCIdleScheduler_Alloc(CThing*) returning CTCIdleScheduler*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCIdleScheduler : public CTCBase {
public:
    CTCIdleScheduler(CThing& thing);                        // 0x004E1537
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x2c - 0xC];              // the class's own members (not modelled); sizeof(CTCIdleScheduler) = 0x2c
};

CTCBase* __fastcall CTCIdleScheduler::Alloc(CThing& thing)
{
    return new CTCIdleScheduler(thing);
}
