// CTCDeadShotInThingTracker::FrameUpdate() -- kill the stuck shot once the thing it is stuck in is gone, dead or moving
// Leaf: FrameUpdate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E34E0, length 70 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x3); CThing::Kill(bool) @ 0x004C9B80.
// CTCDeadShotInThingTracker vtable 0x0127040C slot 9 (rtti-vtables.tsv; chunk label
// "CTCDeadShotInThingTracker::vfunc_9"); slot 9 of the CTCBase-derived vtables is FrameUpdate (93 classes).
// PDB CTCDeadShotInThingTracker: PThingStuckIn +0x0c, HasThingStuckIn +0x14. Retail CThing (PDB - 0x10 from the
// component map on): the Dead bit at +0x91 bit 0 (PDB +0xa1), PhysicsTC at +0x60 (PDB +0x70).
// engine/CTCPhysicsBase.h: the +0x3c bitfield group (the header names it after OldMoving) -- bit 0 is Moving.
// Shape: the flag is loaded into al and tested; the item is fetched three times through the out-of-line
// GetPItem (null, the dead bit, then the physics component's Moving bit); the three-way `||` shares the
// Kill(true) tail and the false exits land on the two pops.
#include "engine/CTCPhysicsBase.h"

struct CThing {
    unsigned char   _pad0[0x60];
    CTCPhysicsBase* PhysicsTC;                              // +0x60
    unsigned char   _pad1[0x91 - 0x64];
    unsigned char   m_flags91;                              // +0x91 bit 0: Dead
    bool IsDead() const { return (m_flags91 & 1) != 0; }
    void Kill(bool immediately);                            // 0x004C9B80
};

struct CBaseIntelligentPointer {                            // sizeof 8: vptr + PData
    virtual void v00();
    void* PData;                                            // +0x04 CBaseObjectPointer*
    CThing* GetPItem() const;                               // 0x00A01B50
};

struct CTCDeadShotInThingTracker {
    virtual void v00();
    CThing*                 ParentThing;                    // +0x04 (CTCBase)
    bool                    ToBeRemoved;                    // +0x08
    bool                    IsInGlobalUpdateTCs;            // +0x09
    CBaseIntelligentPointer PThingStuckIn;                  // +0x0c CIntelligentPointer<const CThing>
    bool                    HasThingStuckIn;                // +0x14
    void FrameUpdate();
};

void CTCDeadShotInThingTracker::FrameUpdate()
{
    if (HasThingStuckIn) {
        if (PThingStuckIn.GetPItem() == 0
            || PThingStuckIn.GetPItem()->IsDead()
            || (PThingStuckIn.GetPItem()->PhysicsTC->OldMoving[0] & 1) != 0)
            ParentThing->Kill(true);
    }
}
