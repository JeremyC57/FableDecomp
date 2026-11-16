#pragma optimize("y", off)
#pragma optimize("s", on)
// CTCParticleAttachmentTracker::CTCParticleAttachmentTracker(CThing&) -- CThingComponent(owner), this vtable, the member list constructed (sentinel node), nothing else.
// Leaf: ??0CTCParticleAttachmentTracker@@QAE@AAVCThing@@@Z
// Ported 2026-11-16 onto the shared engine/CTCBase.h: the local base model CThingComponent (which bound ??0CThingComponent@@QAE@PAVCThing@@@Z to 0x00686800) is gone;
//   the base call is ??0CTCBase@@QAE@AAVCThing@@@Z and this constructor takes CThing& as the PDBs spell it (??0CTCParticleAttachmentTracker@@QAE@AAVCThing@@@Z).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DED6F, length 39 bytes; frame-pointer size TU (`push ebp ; mov ebp,esp`,
// `push dword ptr [ebp+8]` pass-through): `optimize("y",off)` + `("s",on)`.
// Callees: CThingComponent::CThingComponent(CThing*) @ 0x00686800 (manifest "CEngineSubPrimitive2DClockSprite", a
// propagated label; every CTC constructor in this module calls it), std::list ctor @ 0x004B8920 (node 0x10).
// Vtable 0x01241AB4 = CTCParticleAttachmentTracker (rtti-vtables.tsv); the manifest label "CCardPositionsDef" is propagated and rejected.
// PDB CTCParticleAttachmentTracker: the list member at +0x0C (12 bytes in the PDB, 4 in retail: the head pointer only)
// Retail std::list (VC6-era Dinkumware, one sentinel-node pointer): the constructor takes the allocator by
// const reference (`explicit list(const allocator&)`), so the caller passes the ADDRESS of an empty allocator
// temporary. The temporary has a user constructor that writes nothing, which is why VC homes it in the high
// byte of the already-consumed argument slot (`lea eax,[ebp+0Bh]`) instead of reserving a stack slot.
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                     // CTCBase(CThing&) 0x00686800 ??0CTCBase@@QAE@AAVCThing@@@Z
struct CRetailAllocator { CRetailAllocator() { } };

class CThing;

struct CRetailList_node0x10 {                   // retail std::list, one sentinel-node pointer
    void* head;
    CRetailList_node0x10(const CRetailAllocator& al = CRetailAllocator());   // 0x004B8920
};

class CTCParticleAttachmentTracker : public CTCBase { public:
    CRetailList_node0x10 AttachedParticles;                  // +0x0C
    CTCParticleAttachmentTracker(CThing& owner);
};

CTCParticleAttachmentTracker::CTCParticleAttachmentTracker(CThing& owner)
    : CTCBase(owner), AttachedParticles()
{
}