// CTCPhysicsStandard::SetFacingAngleXY(float angle) -- set one facing Euler angle (component 0) keeping the other two from the current set: snapshot the old set, rebuild RHSet from the three angles and mark the thing moving
// Leaf: SetFacingAngleXY
// Retail: Fable.exe (TLC, VC7.1) @ 0x00723760, 115 bytes. CTCPhysicsStandard vtable 0x01265F84 slot 66. The catalog labels all three
//   setters (slots 66/67/68 = 0x00723760/0x007237E0/0x00723860) "SetFacingAngleXY"; slot 66 sets angle X (the XY-plane heading
//   read back by slot 63 from RHSet.Forward), so the names of slots 67/68 and getters 64/65 (YZ / XZ) are inferred by position.
// Callees: virtual slots 95 (+0x17c UpdateOldRHSet, 0x00723930), 63 (+0xfc, 0x00723B30), 64 (+0x100, 0x00723B90), 65 (+0x104,
//   0x00723750); CRightHandedSet::CRightHandedSet(const C3DVector& eulerAngles) @ 0x00A229E0 (catalog "BuildEulerRotationMatrix";
//   thiscall on the frame temporary, returns it); CTCPhysicsBase::SetAsMoving(bool) @ 0x006B0130.
// Layout: CTCPhysicsStandard RHSet +0x50 (PDB, as the landed SetFacingAnglesFromRHSet 0x007238E0).
// Shape: the angle vector is filled member by member in retail store order (here Y, X, Z: the argument is an integer copy);
//   `RHSet = CRightHandedSet(angles)` copies the 24-byte temporary from eax through a `lea ecx,[esi+50h]` base.
class C3DVector { public: float X, Y, Z; };
class CRightHandedSet { public:
    C3DVector Up;
    C3DVector Forward;
    CRightHandedSet() {}
    CRightHandedSet(const C3DVector& eulerAngles);   // 0x00A229E0
};
class CThingPhysicsMeshInfo;
class CTCPhysicsBase { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041();
    virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053();
    virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062();
    virtual float GetFacingAngleXY();   // slot 63 (+0xfc)
    virtual float GetFacingAngleYZ();   // slot 64 (+0x100)
    virtual float GetFacingAngleXZ();   // slot 65 (+0x104)
    virtual void SetFacingAngleXY(float angle);   // slot 66 (+0x108)
    virtual void SetFacingAngleYZ(float angle);   // slot 67 (+0x10c)
    virtual void SetFacingAngleXZ(float angle);   // slot 68 (+0x110)
    virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080();
    virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086();
    virtual void v087(); virtual void v088(); virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092();
    virtual void v093(); virtual void v094();
    virtual void UpdateOldRHSet();   // slot 95 (+0x17c)
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
    virtual void SetFacingAngleXY(float angle);
};

void CTCPhysicsStandard::SetFacingAngleXY(float angle)
{
    UpdateOldRHSet();
    C3DVector angles;
    angles.Y = GetFacingAngleYZ();
    angles.X = angle;
    angles.Z = GetFacingAngleXZ();
    RHSet = CRightHandedSet(angles);
    SetAsMoving(true);
}