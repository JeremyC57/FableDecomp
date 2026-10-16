// CTCPhysicsStandard::SetPhysicsEnabled(bool enabled) -- store the PhysicsEnabled flag; enabling clears the velocity, marks the thing moving, clears OnGround/OnMesh and the thing stood on; disabling stops it (Moving off, OldMoving on)
// Leaf: SetPhysicsEnabled
// Retail: Fable.exe (TLC, VC7.1) @ 0x00723630, 99 bytes. CTCPhysicsStandard vtable 0x01265F84 slot 36 (catalog "vfunc_36"; the
//   name follows the PDB bit it stores: +0x85 bit 2 = PhysicsEnabled).
// Callees: CTCPhysicsBase::SetAsMoving(bool) @ 0x006B0130, CTCPhysicsBase::SetThingStandingOn(CTCPhysicsBase*) @ 0x006AF6D0.
// Layout: PDB CTCPhysicsBase / CTCPhysicsStandard (as the landed 0x00723930): Velocity +0x28, Moving/OldMoving +0x3c bits 0/1,
//   OnGround/OnMesh +0x84 bits 4/5, PhysicsEnabled +0x85 bit 2.
// Shape: the velocity is cleared by a chained `Z = Y = X = 0` style store run (highest address first); the two OnGround/OnMesh
//   clears merge into one `and byte ptr [esi+84h],0CFh`, the two Moving bits into one load/and/or/store.
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
    void SetAsMoving(bool moving);              // 0x006B0130
    void SetThingStandingOn(CTCPhysicsBase* standingOn);   // 0x006AF6D0
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
    virtual void SetPhysicsEnabled(bool enabled);
};

void CTCPhysicsStandard::SetPhysicsEnabled(bool enabled)
{
    PhysicsEnabled = enabled;
    if (enabled) {
        Velocity.X = Velocity.Y = Velocity.Z = 0.0f;
        SetAsMoving(true);
        OnGround = false;
        OnMesh = false;
        SetThingStandingOn(0);
        return;
    }
    Moving = false;
    OldMoving = true;
}
