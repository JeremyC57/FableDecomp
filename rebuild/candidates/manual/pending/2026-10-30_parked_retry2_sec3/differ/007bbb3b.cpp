// NThingShot::CTCShotDefinitionProjectile::InitMovement() -- aim the projectile: the direction from the shot to its target, scattered by the shot's accuracy and normalised, gives the shot's orientation (up = direction x its horizontal perpendicular) and its launch acceleration (direction times the initial velocity), then gravity is switched off
// Grade: DIFFER(269v268): 1 instruction -- (float)(double) per component in the member operator- and a named randomised result give retail's 0x58 frame; residue: retail copies the randomised direction through the returned pointer (mov esi,eax), ours through the named slot (lea esi,[ebp-40h])
// Leaf: InitMovement
// Retail: Fable.exe (TLC, VC7.1) @ 0x007BBB3B, 268 bytes (plain `ret`). NThingShot::CTCShotDefinitionProjectile vtable
//   0x01240CCC slot 33. The shot TU is size-optimised with a frame pointer: `optimize("y", off)` + `("s", on)`.
// Callees: CTCShotDefinitionBase::GetTargetPos() const @ 0x007BEDD0 (C3DVector by hidden pointer; the landed fastcall model
//   names it CTCPhysicsRigidBody::GetFacingVector), __ftol2 @ 0x00BFEA70, CTCBase::GetWorld() @ 0x00686A70,
//   CWorld::GetPRandomSeed() @ 0x0049D810, RandomiseShotDirection(const C3DVector&, long, unsigned long*) @ 0x007BEBE0
//   (`__fastcall`, the hidden result pointer in ecx), C3DVector::Normalise() @ 0x00A14440 (twice), CRightHandedSet(const
//   C3DVector& up, const C3DVector& forward) @ 0x00487050, the physics body's SetOrientation (slot 32, +0x80),
//   CTCShotDefinitionBase::GetInitialVelocity() @ 0x007BEFB0 (float in st0), CTCPhysicsBase::ApplyAcceleration(const
//   C3DVector&) @ 0x006B0210, EnableGravity(bool) (slot 38, +0x98). 0.0f from the pool 0x0122DEDC.
// Layout: Ego_r.pdb CTCShotDefinitionBase AccuracyPercentage +0x40 (PDB 0x44); retail CThing PhysicsTC +0x60;
//   CTCPhysicsBase Position +0x0C.
// Shape: the target position and the truncated accuracy are named locals (the target is read back from its frame slot,
//   the accuracy truncated before the seed is fetched); C3DVector arithmetic through inline operators returning `C3DVector(...)` (constructor arguments evaluated Z first);
//   the up vector is the inline cross product of the direction with (dir.Y, -dir.X, 0); `this` is kept in a frame word
//   across the movsd copy of the scattered direction.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    __forceinline C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    __forceinline C3DVector operator-(const C3DVector& o) const { return C3DVector((float)(double)(X - o.X), (float)(double)(Y - o.Y), (float)(double)(Z - o.Z)); }
    __forceinline C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    C3DVector& Normalise();                             // 0x00A14440
};
__forceinline C3DVector CrossProduct(const C3DVector& a, const C3DVector& b)
{
    return C3DVector(b.Y * a.Z - a.Y * b.Z, a.X * b.Z - b.X * a.Z, a.Y * b.X - a.X * b.Y);
}
struct CRightHandedSet {
    C3DVector Up;
    C3DVector Forward;
    CRightHandedSet(const C3DVector& up, const C3DVector& forward);   // 0x00487050
};
C3DVector __fastcall RandomiseShotDirection(const C3DVector& direction, long accuracy, unsigned long* seed);   // 0x007BEBE0
struct CWorld {
    unsigned long* GetPRandomSeed();                    // 0x0049D810
};
struct CTCPhysicsBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31();
    virtual void SetOrientation(const CRightHandedSet& rhSet);   // slot 32 (+0x80)
    virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37();
    virtual void EnableGravity(bool enable);            // slot 38 (+0x98)
    void*     ParentThing;                              // +0x04
    unsigned char _pad08[0x4];
    C3DVector Position;                                 // +0x0c
    void ApplyAcceleration(const C3DVector& acceleration);   // 0x006B0210
};
class CThing {
public:
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                          // +0x60
};
namespace NThingShot {
struct CTCShotDefinitionProjectile {
    virtual void v00();
    CThing*       ParentThing;                          // +0x04 (CTCBase)
    unsigned char _pad08[0x40 - 0x08];
    float         AccuracyPercentage;                   // +0x40
    CWorld* GetWorld();                                 // 0x00686A70 (CTCBase)
    C3DVector GetTargetPos() const;                     // 0x007BEDD0 (CTCShotDefinitionBase)
    float GetInitialVelocity();                         // 0x007BEFB0 (CTCShotDefinitionBase)
    void InitMovement();
};

void CTCShotDefinitionProjectile::InitMovement()
{
    CTCPhysicsBase* physics = ParentThing->PhysicsTC;
    C3DVector target = GetTargetPos();
    C3DVector direction = target - physics->Position;
    long accuracy = (long)AccuracyPercentage;
    C3DVector randomised = RandomiseShotDirection(direction, accuracy, GetWorld()->GetPRandomSeed());
    direction = randomised;
    direction.Normalise();
    C3DVector up = CrossProduct(direction, C3DVector(direction.Y, -direction.X, 0.0f));
    up.Normalise();
    physics->SetOrientation(CRightHandedSet(up, direction));
    C3DVector acceleration = direction * GetInitialVelocity();
    physics->ApplyAcceleration(acceleration);
    physics->EnableGravity(false);
}
}
