// CTCPhysicsStandard::UpdateOnSurface(const vector<CThing*>& nearbyThings, const C3DVector& position) -- recompute the on-mesh / on-ground state and the thing stood on at the position, store the two flags, and record the thing stood on (none while on the ground)
// Leaf: UpdateOnSurface
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E55E0, 143 bytes.
// Callees: CTCPhysicsStandard::GetOnSurfaceParameters(const vector<CThing*>&, bool&, bool&, const C3DVector&, CTCPhysicsBase*&)
//   @ 0x008E5480, CTCPhysicsBase::SetThingStandingOn(CTCPhysicsBase*) @ 0x006AF6D0.
// Layout: CTCPhysicsStandard +0x84 bit 4 OnGround, bit 5 OnMesh (PDB).
// Shape: the two flags are copied into bool locals (frame bytes 2/3) and the stood-on pointer zeroed (frame word 4) before the
//   call; the write-back of both bit fields merges into one load/and/or/store; the argument is `OnGround ? 0 : standingOn`
//   computed from the stored byte (`and al,10h ; neg ; sbb ; not ; and`).
class C3DVector { public: float X, Y, Z; };
class CRightHandedSet { public: C3DVector Up; C3DVector Forward; };
class CThingPhysicsMeshInfo;
class CThing;
struct CThingVector { CThing** _M_start; CThing** _M_finish; CThing** _M_end_of_storage; };   // STLport vector<CThing*>
class CTCPhysicsBase { public:
    virtual ~CTCPhysicsBase();
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
    void GetOnSurfaceParameters(const CThingVector& nearbyThings, bool& onMesh, bool& onGround, const C3DVector& position, CTCPhysicsBase*& standingOn);   // 0x008E5480
    void UpdateOnSurface(const CThingVector& nearbyThings, const C3DVector& position);
};

void CTCPhysicsStandard::UpdateOnSurface(const CThingVector& nearbyThings, const C3DVector& position)
{
    bool onMesh = OnMesh;
    bool onGround = OnGround;
    CTCPhysicsBase* standingOn = 0;
    GetOnSurfaceParameters(nearbyThings, onMesh, onGround, position, standingOn);
    OnMesh = onMesh;
    OnGround = onGround;
    SetThingStandingOn(OnGround ? 0 : standingOn);
}