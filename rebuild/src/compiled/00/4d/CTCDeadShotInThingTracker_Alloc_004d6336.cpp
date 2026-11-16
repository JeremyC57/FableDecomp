#pragma optimize("s",on)
// CTCDeadShotInThingTracker::Alloc(CThing& thing) -- the registry factory: a new CTCDeadShotInThingTracker for `thing` (operator new(0x18) 0x00BFEA1A, null-checked, then
//   the constructor 0x007E3450), returned as its CTCBase
// Leaf: ?Alloc@CTCDeadShotInThingTracker@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D6336, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x18 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCDeadShotInThingTracker::CTCDeadShotInThingTracker ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCDeadShotInThingTracker@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCDeadShotInThingTracker@@QAE@AAVCThing@@@Z (0x007E3450, landed). (Landed earlier as a generated free function CTCDeadShotInThingTracker_Alloc(CThing*) returning CTCDeadShotInThingTracker*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCDeadShotInThingTracker : public CTCBase {
public:
    CTCDeadShotInThingTracker(CThing& thing);                        // 0x007E3450
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x18 - 0xC];              // the class's own members (not modelled); sizeof(CTCDeadShotInThingTracker) = 0x18
};

CTCBase* __fastcall CTCDeadShotInThingTracker::Alloc(CThing& thing)
{
    return new CTCDeadShotInThingTracker(thing);
}
