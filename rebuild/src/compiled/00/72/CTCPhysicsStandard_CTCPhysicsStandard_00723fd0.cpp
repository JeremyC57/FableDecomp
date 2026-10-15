// CTCPhysicsStandard::CTCPhysicsStandard(CThing& owner) -- CTCPhysicsBase(owner), no physics mesh info, RHSetLastFrameSet 0, all physics flags clear except FeelsGravity and PhysicsEnabled, then this class's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00723FD0, length 50 bytes.
// Leaf: ??0CTCPhysicsStandard
// Callees: CTCPhysicsBase::CTCPhysicsBase(CThing&) @ 0x006B0730. Vtable 0x01265F84 = CTCPhysicsStandard (rtti-vtables.tsv); the
// chunk label "CTCActivationTrigger::CTCActivationTrigger" is propagated and rejected. Ego_r.pdb CTCPhysicsStandard (0x88
// bytes): PPhysicsMeshInfo +0x40, OffsetVector3D +0x44, RHSet +0x50, OldRHSet +0x68, RHSetLastFrameSet +0x80, then sixteen
// `bool : 1` flags in the bytes +0x84 / +0x85 (FeelsGravity and PhysicsEnabled are bit 2 of each).
// Shape: every bitfield of both bytes is initialised, so VC7.1 folds each byte's eight bit stores into one constant store
// (`mov al,4` shared by the two bytes); all initialisers sit before the vtable store (plain members, no member-object
// constructors). engine/CTCPhysicsStandard.h / CTCPhysicsBase.h are flattened PDB PODs (no virtuals, opaque bitfield bytes),
// so the polymorphic classes are modelled locally with the same offsets.
struct CThing;
struct CThingPhysicsMeshInfo;
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector Up; C3DVector Forward; };

struct CTCPhysicsBase {                        // vtable 0x0125D2C4, 0x40 bytes
    virtual void v00();
    unsigned char _body[0x3c];                 // +0x04 CTCBase .. +0x3c Moving/OldMoving
    CTCPhysicsBase(CThing& owner);             // 0x006B0730
};

struct CTCPhysicsStandard : CTCPhysicsBase {   // vtable 0x01265F84
    CThingPhysicsMeshInfo* PPhysicsMeshInfo;   // +0x40
    C3DVector       OffsetVector3D;            // +0x44
    CRightHandedSet RHSet;                     // +0x50
    CRightHandedSet OldRHSet;                  // +0x68
    long            RHSetLastFrameSet;         // +0x80
    bool AlwaysFacingMovementDirection : 1;    // +0x84 bit 0
    bool AngledWithGround : 1;
    bool FeelsGravity : 1;
    bool FeelsAirResistance : 1;
    bool OnGround : 1;
    bool OnMesh : 1;
    bool InteractsWithThings : 1;
    bool CollidesWithThings : 1;
    bool CollidesWithThingsOneWay : 1;         // +0x85 bit 0
    bool CollidesWithLandscape : 1;
    bool PhysicsEnabled : 1;
    bool Immoveable : 1;
    bool NavigationLayers : 1;
    bool PushableByHero : 1;
    bool ExcludingCreatures : 1;
    bool Impassable : 1;
    virtual void v00();
    CTCPhysicsStandard(CThing& owner);
};

CTCPhysicsStandard::CTCPhysicsStandard(CThing& owner)
    : CTCPhysicsBase(owner), PPhysicsMeshInfo(0), RHSetLastFrameSet(0),
      AlwaysFacingMovementDirection(false), AngledWithGround(false), FeelsGravity(true), FeelsAirResistance(false),
      OnGround(false), OnMesh(false), InteractsWithThings(false), CollidesWithThings(false),
      CollidesWithThingsOneWay(false), CollidesWithLandscape(false), PhysicsEnabled(true), Immoveable(false),
      NavigationLayers(false), PushableByHero(false), ExcludingCreatures(false), Impassable(false)
{
}