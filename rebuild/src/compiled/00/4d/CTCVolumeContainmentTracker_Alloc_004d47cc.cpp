#pragma optimize("s",on)
// CTCVolumeContainmentTracker::Alloc(CThing& thing) -- the registry factory: a new CTCVolumeContainmentTracker for `thing` (operator new(0x24) 0x00BFEA1A, null-checked, then
//   the constructor 0x00785110), returned as its CTCBase
// Leaf: ?Alloc@CTCVolumeContainmentTracker@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D47CC, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x24 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCVolumeContainmentTracker::CTCVolumeContainmentTracker ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCVolumeContainmentTracker@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCVolumeContainmentTracker@@QAE@AAVCThing@@@Z (0x00785110, landed). (Landed earlier as a generated free function CTCVolumeContainmentTracker_Alloc(CThing*) returning CTCVolumeContainmentTracker*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCVolumeContainmentTracker : public CTCBase {
public:
    CTCVolumeContainmentTracker(CThing& thing);                        // 0x00785110
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x24 - 0xC];              // the class's own members (not modelled); sizeof(CTCVolumeContainmentTracker) = 0x24
};

CTCBase* __fastcall CTCVolumeContainmentTracker::Alloc(CThing& thing)
{
    return new CTCVolumeContainmentTracker(thing);
}
