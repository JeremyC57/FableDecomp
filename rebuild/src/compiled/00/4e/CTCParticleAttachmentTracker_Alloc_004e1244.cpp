#pragma optimize("s",on)
// CTCParticleAttachmentTracker::Alloc(CThing& thing) -- the registry factory: a new CTCParticleAttachmentTracker for `thing` (operator new(0x10) 0x00BFEA1A, null-checked, then
//   the constructor 0x004DED6F), returned as its CTCBase
// Leaf: ?Alloc@CTCParticleAttachmentTracker@@SIPAVCTCBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E1244, oracle length 29 bytes.
// Retail bytes:  push esi ; push 0x10 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCParticleAttachmentTracker::CTCParticleAttachmentTracker ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CTCParticleAttachmentTracker@@SIPAVCTCBase@@AAVCThing@@@Z (public static __fastcall); the constructor is
//   ??0CTCParticleAttachmentTracker@@QAE@AAVCThing@@@Z (0x004DED6F, landed). (Landed earlier as a generated free function CTCParticleAttachmentTracker_Alloc(CThing*) returning CTCParticleAttachmentTracker*;
//   re-authored 2026-11-16 on the shared engine/CTCBase.h, same bytes: CTCBase is the first base, so the upcast needs no adjustment.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCParticleAttachmentTracker : public CTCBase {
public:
    CTCParticleAttachmentTracker(CThing& thing);                        // 0x004DED6F
    static CTCBase* __fastcall Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
private:
    unsigned char Members[0x10 - 0xC];              // the class's own members (not modelled); sizeof(CTCParticleAttachmentTracker) = 0x10
};

CTCBase* __fastcall CTCParticleAttachmentTracker::Alloc(CThing& thing)
{
    return new CTCParticleAttachmentTracker(thing);
}
