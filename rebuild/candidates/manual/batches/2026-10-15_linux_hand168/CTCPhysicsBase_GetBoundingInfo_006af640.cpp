// CTCPhysicsBase::GetBoundingInfo(C3DVector& centre, float& radius) -- the bounding sphere in world space: the physics mesh's sphere offset by the position when there is one, otherwise the position raised by the thing's radius
// Leaf: GetBoundingInfo
// Retail: Fable.exe (TLC, VC7.1) @ 0x006AF640, 139 bytes.
// Callees: virtual slot 93 (+0x174) of CTCPhysicsBase (0x006B13B0 in the CTCPhysicsBase vtable), returning the local bounding
//   physics mesh info (PDB CThingPhysicsMeshInfo: ToCentre +0x08, BoundingRadius +0x14) or null.
// Layout: engine/CTCPhysicsBase.h: Position +0x0c, Radius +0x34 (modelled locally to declare the virtual slot).
// Shape: the mesh arm `centre = C3DVector(...)` built in the frame and copied through integer registers (3-float ctor only); the
//   radius is stored first in the fallback arm, the position copy then `centre.Z += radius` read back through the reference.
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
struct CThingPhysicsMeshInfo {
    unsigned char _pad00[0x08];
    C3DVector ToCentre;                         // +0x08
    float     BoundingRadius;                   // +0x14
};
struct CThing;
struct CTCPhysicsBase {
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
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065();
    virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077();
    virtual void v078(); virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083();
    virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089();
    virtual void v090(); virtual void v091(); virtual void v092();
    virtual const CThingPhysicsMeshInfo* GetPhysicsMeshInfo();   // slot 93 (+0x174)
    CThing*   ParentThing;                      // +0x04
    unsigned char _pad08[0x4];
    C3DVector Position;                         // +0x0c
    C3DVector OldPosition;                      // +0x18
    long      OldPositionLastFrameSet;          // +0x24
    C3DVector Velocity;                         // +0x28
    float     Radius;                           // +0x34
    void GetBoundingInfo(C3DVector& centre, float& radius);
};

void CTCPhysicsBase::GetBoundingInfo(C3DVector& centre, float& radius)
{
    const CThingPhysicsMeshInfo* sphere = GetPhysicsMeshInfo();
    if (sphere) {
        centre = C3DVector(sphere->ToCentre.X + Position.X, sphere->ToCentre.Y + Position.Y, sphere->ToCentre.Z + Position.Z);
        radius = sphere->BoundingRadius;
        return;
    }
    radius = Radius;
    centre = Position;
    centre.Z += radius;
}
