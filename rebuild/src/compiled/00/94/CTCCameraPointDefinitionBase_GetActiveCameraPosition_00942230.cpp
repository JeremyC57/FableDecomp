// CTCCameraPointDefinitionBase::GetActiveCameraPosition(const CThing* thing) -- catalog label; the body is the constructor of a bounding sphere taken from a thing: its position and its physics radius
// Leaf: ??0CThingBoundingSphere
//
// Retail 0x00942230, 47 bytes (`ret 4`, returns `this`). Callee: CThing::GetPosition() const
// @ 0x004C73D0 (returns a const C3DVector&). Called from CAIStateGroup_KidReactToBlockage::IsRunnable
// (0x00941FC0), so the camera-point label is propagated over an unrelated body; the class is named
// here by what it builds (CThingBoundingSphere: a centre at +0 and a radius at +0xc).
// The thing's physics component is CThing +0x60 (PhysicsTC, Ego_r.pdb +0x70) and its Radius is
// CTCPhysicsBase +0x34 (Ego_r.pdb +0x34).
// Shape: a constructor (`mov eax,esi` at the end) storing the position into three SEPARATE float
// members (dword moves through ecx/edx/eax straight off `this` in esi) and then the radius; the thing
// argument is kept in edi across the call. A C3DVector member copied as a struct materialises its
// address first (`mov ecx,esi ; mov [ecx],edx`, 49v47).

struct C3DVector { float X, Y, Z; };

struct CTCPhysicsBase
{
    unsigned char _pad_0x00[0x34];
    float         Radius;                           // +0x34
};

struct CThing
{
    unsigned char   _pad_0x00[0x60];
    CTCPhysicsBase* PhysicsTC;                      // +0x60
    const C3DVector& GetPosition() const;           // 0x004C73D0
};

struct CThingBoundingSphere
{
    float CentreX;                                  // +0x00
    float CentreY;                                  // +0x04
    float CentreZ;                                  // +0x08
    float Radius;                                   // +0x0c

    CThingBoundingSphere(const CThing* thing);
};

CThingBoundingSphere::CThingBoundingSphere(const CThing* thing)
{
    const C3DVector& position = thing->GetPosition();
    CentreX = position.X;
    CentreY = position.Y;
    CentreZ = position.Z;
    Radius = thing->PhysicsTC->Radius;
}