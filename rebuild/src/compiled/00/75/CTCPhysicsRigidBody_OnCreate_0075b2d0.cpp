// CTCPhysicsRigidBody::OnCreate() -- start with OldRHSet = RHSet and a zero offset, build the physics mesh info from the parent's graphic data when it has a graphic appearance, take the offset from the mesh info's centre (XY), run the base OnCreate and set up the rigid body
// Leaf: OnCreate
// Retail: Fable.exe (TLC, VC7.1) @ 0x0075B2D0, 127 bytes. CTCPhysicsRigidBody vtable 0x0126996C slot 4 (OnCreate).
// Callees: CTCPhysicsBase::CreatePPhysicsMeshInfo(const CGraphicDataRef&) @ 0x006B05A0, virtual slot 93 (+0x174,
//   GetPhysicsMeshInfo; the CTCPhysicsRigidBody override is 0x0075BFD0), CTCPhysicsBase::OnCreate @ 0x006B0780 (called
//   non-virtually), CTCPhysicsRigidBody::InitialiseBody @ 0x0075B000 (tail jump).
// Layout: as the ctor 0x0075AA80: PPhysicsMeshInfo +0x64, RHSet +0x68, OldRHSet +0x80, OffsetVector +0x98 (C2DVector);
//   CThing +0x64 PGraphicAppearance, whose graphic data reference sits at +0x44; CThingPhysicsMeshInfo ToCentre +0x08.
// Shape: the set copy is an implicit POD assignment; the zero offset is an inline `OffsetVector.Set(0.0f, 0.0f)` member
//   (two stores from one zero register; stores through the member's `this` keep the ParentThing load AFTER them, as
//   retail -- direct member stores let VC hoist `mov edx,[esi+4]` above the last RHSet pair; a `C2DVector(0, 0)`
//   temporary goes through the frame); the offset copy takes &info->ToCentre and copies X, Y as dwords.
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector Up; C3DVector Forward; };
struct C2DVector {
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    void Set(float x, float y) { X = x; Y = y; }
};
struct CThingPhysicsMeshInfo {
    unsigned short BankIndex;                   // +0x00
    float          RenderSizeX;                 // +0x04
    C3DVector      ToCentre;                    // +0x08
};
struct CGraphicDataRef;
struct CTCGraphicAppearance {
    unsigned char _pad00[0x44];
    unsigned char GraphicData[0x10];            // +0x44 the graphic data reference (index +0, type byte +0xd)
};
struct CThing {
    unsigned char _pad00[0x64];
    CTCGraphicAppearance* PGraphicAppearance;   // +0x64
};
struct CTCPhysicsBase {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003();
    virtual void OnCreate();   // slot 4 (+0x10)
    virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010();
    virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016();
    virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022();
    virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028();
    virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034();
    virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040();
    virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046();
    virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052();
    virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058();
    virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064();
    virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070();
    virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076();
    virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082();
    virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088();
    virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092();
    virtual const CThingPhysicsMeshInfo* GetPhysicsMeshInfo();   // slot 93 (+0x174)
    CThing*   ParentThing;                      // +0x04
    unsigned char _pad08[0x38];
    void CreatePPhysicsMeshInfo(const CGraphicDataRef& graphic);   // 0x006B05A0
};
struct CTCPhysicsRigidBody : CTCPhysicsBase {
    void*           CollisionNormal[3];         // +0x40
    void*           CollidingVertexIndex[3];    // +0x4c
    void*           Collidors[3];               // +0x58
    CThingPhysicsMeshInfo* PPhysicsMeshInfo;    // +0x64
    CRightHandedSet RHSet;                      // +0x68
    CRightHandedSet OldRHSet;                   // +0x80
    C2DVector       OffsetVector;               // +0x98
    void InitialiseBody();                      // 0x0075B000
    virtual void OnCreate();
};

void CTCPhysicsRigidBody::OnCreate()
{
    OldRHSet = RHSet;
    OffsetVector.Set(0.0f, 0.0f);
    CTCGraphicAppearance* graphic = ParentThing->PGraphicAppearance;
    if (graphic)
        CreatePPhysicsMeshInfo(*(const CGraphicDataRef*)graphic->GraphicData);
    const C3DVector& centre = GetPhysicsMeshInfo()->ToCentre;
    OffsetVector.X = centre.X;
    OffsetVector.Y = centre.Y;
    CTCPhysicsBase::OnCreate();
    InitialiseBody();
}