// CTCPhysicsStandard::SetInteractionFlags(unsigned long flags) -- copy the physics-interaction flag word into the sixteen bool:1 flags; dropping Immoveable re-enables physics
// Leaf: SetInteractionFlags
// Retail: Fable.exe (TLC, VC7.1) @ 0x007234C0, 176 bytes (`ret 4`). CTCPhysicsStandard vtable 0x01265F84 slot 43 (catalog
//   "vfunc_43"; GOTCHAS chunk 168 names slot 43 SetInteractionFlags).
// Callees: virtual SetPhysicsEnabled(bool) slot 36 (+0x90, landed 0x00723630).
// Layout: Ego_r.pdb CTCPhysicsStandard flag bytes +0x84 (AlwaysFacingMovementDirection .. CollidesWithThings, bits 0-7) and +0x85
//   (CollidesWithThingsOneWay .. Impassable). engine/CTCPhysicsStandard.h flattens those bytes and carries no virtuals, so the
//   hierarchy is modelled locally with the same offsets (as the landed SetPhysicsEnabled 0x00723630).
// Shape: one assignment per flag from `(flags >> bit) & 1`; VC7.1 merges every store into one byte into a single shifted/or-ed
//   build plus `and cl,3Ch` keep mask (FeelsGravity/FeelsAirResistance/OnGround/OnMesh, PhysicsEnabled/NavigationLayers kept).
//   The old Immoveable is read from the merged byte before its own `xor/and 8/xor` store, then `old && !new` calls slot 36.
//   Flag word bits used: 1 landscape, 2 things, 3 things one-way, 4 interacts, 5 immoveable, 6 facing, 8 angled, 10 excluding
//   creatures, 11 impassable.
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector Up; C3DVector Forward; };
struct CThingPhysicsMeshInfo;
struct CTCPhysicsBase {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void SetPhysicsEnabled(bool enabled);   // slot 36 (+0x90)
    virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042();
    virtual void SetInteractionFlags(unsigned long flags);   // slot 43 (+0xac)
    void*     ParentThing;                      // +0x04 (CTCBase)
    unsigned char _pad08[0x4];
    C3DVector Position;                         // +0x0c
    C3DVector OldPosition;                      // +0x18
    long      OldPositionLastFrameSet;          // +0x24
    C3DVector Velocity;                         // +0x28
    float     Radius;                           // +0x34
    void*     PhysicsDef;                       // +0x38 CDefPointer<CPhysicsDef const>::Object
    bool Moving : 1;                            // +0x3c bit 0
    bool OldMoving : 1;                         // +0x3c bit 1
};
struct CTCPhysicsStandard : CTCPhysicsBase {
    CThingPhysicsMeshInfo* PPhysicsMeshInfo;    // +0x40
    C3DVector       OffsetVector3D;             // +0x44
    CRightHandedSet RHSet;                      // +0x50
    CRightHandedSet OldRHSet;                   // +0x68
    long            RHSetLastFrameSet;          // +0x80
    bool AlwaysFacingMovementDirection : 1;     // +0x84 bit 0
    bool AngledWithGround : 1;                  //       bit 1
    bool FeelsGravity : 1;                      //       bit 2
    bool FeelsAirResistance : 1;                //       bit 3
    bool OnGround : 1;                          //       bit 4
    bool OnMesh : 1;                            //       bit 5
    bool InteractsWithThings : 1;               //       bit 6
    bool CollidesWithThings : 1;                //       bit 7
    bool CollidesWithThingsOneWay : 1;          // +0x85 bit 0
    bool CollidesWithLandscape : 1;             //       bit 1
    bool PhysicsEnabled : 1;                    //       bit 2
    bool Immoveable : 1;                        //       bit 3
    bool NavigationLayers : 1;                  //       bit 4
    bool PushableByHero : 1;                    //       bit 5
    bool ExcludingCreatures : 1;                //       bit 6
    bool Impassable : 1;                        //       bit 7
    virtual void SetInteractionFlags(unsigned long flags);
};
void CTCPhysicsStandard::SetInteractionFlags(unsigned long flags)
{
    AlwaysFacingMovementDirection = (flags >> 6) & 1;
    AngledWithGround = (flags >> 8) & 1;
    InteractsWithThings = (flags >> 4) & 1;
    CollidesWithThings = (flags >> 2) & 1;
    CollidesWithThingsOneWay = (flags >> 3) & 1;
    CollidesWithLandscape = (flags >> 1) & 1;
    ExcludingCreatures = (flags >> 10) & 1;
    Impassable = (flags >> 11) & 1;
    bool wasImmoveable = Immoveable;
    Immoveable = (flags >> 5) & 1;
    if (wasImmoveable && !Immoveable)
        SetPhysicsEnabled(true);
}