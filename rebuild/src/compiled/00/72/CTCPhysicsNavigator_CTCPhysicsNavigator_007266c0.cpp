// CTCPhysicsNavigator::CTCPhysicsNavigator(CThing* thing) -- CTCPhysicsStandard base, not navigating, AdditionalImpulse zeroed, both forced-move bits cleared
// Retail: Fable.exe (TLC, VC7.1) @ 0x007266C0, 58 bytes. Stores vtable 0x01266314 = CTCPhysicsNavigator (rtti-vtables.tsv), so the
//   catalogue's "CTCPhysicsBase::CTCPhysicsBase" label is wrong.
// Callees: CTCPhysicsStandard::CTCPhysicsStandard @ 0x00723FD0 (manifest "CTCActivationTrigger::CTCActivationTrigger", propagated;
//   CTCPhysicsControlled's constructor calls it too).
// Layout (PDB CTCPhysicsNavigator, 0xb0, packed 1 -- engine/CTCPhysicsNavigator.h carries the same offsets as a flat struct, so the
//   polymorphic class is modelled locally for its constructor): Navigating +0x88, PositionToNavigateTo +0x89, AdditionalImpulse +0x95,
//   ForcedMoveToPos +0xa1, {ForcedMoveToPosSet:1, ForcedMoveToPosUsePhysics:1} +0xad.
// Shape: Navigating (a plain initialiser) is stored before the vtable; AdditionalImpulse is a C3DVector member built by its inline
//   value constructor (after the vtable, from the shared zero register); the two bits are body assignments (one `and 0FCh`).
// Leaf: ??0CTCPhysicsNavigator
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
class CThing;

#include "engine/C3DVector.h"

#pragma pack(push, 1)
struct CTCPhysicsStandard {
    virtual void v000();
    unsigned char _base[0x84];              // +0x04 CTCPhysicsBase + CTCPhysicsStandard members
    CTCPhysicsStandard(CThing* thing);      // 0x00723FD0
};

struct CTCPhysicsNavigator : CTCPhysicsStandard {
    bool      Navigating;                   // +0x88
    C3DVector PositionToNavigateTo;         // +0x89
    C3DVector AdditionalImpulse;            // +0x95
    C3DVector ForcedMoveToPos;              // +0xa1
    bool      ForcedMoveToPosSet : 1;       // +0xad bit 0
    bool      ForcedMoveToPosUsePhysics : 1;// +0xad bit 1
    virtual void v000();
    CTCPhysicsNavigator(CThing* thing);
};
#pragma pack(pop)

CTCPhysicsNavigator::CTCPhysicsNavigator(CThing* thing)
    : CTCPhysicsStandard(thing), Navigating(false), AdditionalImpulse(0.0f, 0.0f, 0.0f)
{
    ForcedMoveToPosSet = false;
    ForcedMoveToPosUsePhysics = false;
}