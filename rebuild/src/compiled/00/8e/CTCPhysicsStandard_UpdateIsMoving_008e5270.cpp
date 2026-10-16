// CTCPhysicsStandard::UpdateIsMoving() -- a moving thing that rests on a stationary surface (or feels no gravity) and whose speed is at most the minimum moving speed stops moving; otherwise it keeps moving unless it is immoveable
// Leaf: UpdateIsMoving
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E5270, 113 bytes.
// Callees: virtual slots 54 (+0xd8, IsOnAnyStationarySurface 0x00723A60, landed), 48 (+0xc0, GetFeelsGravity: the override
//   0x00724C00 reads +0x84 bit 2) and 52 (+0xd0, IsImmoveable: 0x00724C40 reads +0x85 bit 3). Global float 0x0129BA3C = 1e-4
//   (the minimum moving speed, squared inline).
// Layout: CTCPhysicsBase Velocity +0x28, Moving +0x3c bit 0.
// Shape: |v|^2 > min*min through an inline C3DVector::GetMagnitudeSquared(); the stop store is shared by both stop exits.
struct C3DVector {
    float X, Y, Z;
    float GetMagnitudeSquared() const { return X * X + Y * Y + Z * Z; }
};
extern float g_fMinMovingSpeed;                // 0x0129BA3C (1e-4)
struct CRightHandedSet { C3DVector Up; C3DVector Forward; };
struct CThingPhysicsMeshInfo;
struct CTCPhysicsBase {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041();
    virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual bool GetFeelsGravity();   // slot 48 (+0xc0)
    virtual void v049(); virtual void v050(); virtual void v051();
    virtual bool IsImmoveable();   // slot 52 (+0xd0)
    virtual void v053();
    virtual bool IsOnAnyStationarySurface();   // slot 54 (+0xd8)
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
    void UpdateIsMoving();
};

void CTCPhysicsStandard::UpdateIsMoving()
{
    if (Moving) {
        if ((IsOnAnyStationarySurface() || !GetFeelsGravity())
            && !(Velocity.GetMagnitudeSquared() > g_fMinMovingSpeed * g_fMinMovingSpeed)) {
            Moving = false;
            return;
        }
        if (!IsImmoveable()) {
            Moving = true;
            return;
        }
        Moving = false;
    }
}