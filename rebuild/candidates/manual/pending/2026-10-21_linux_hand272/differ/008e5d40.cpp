// CTCPhysicsStandard::Move() -- one physics step: apply forces, gather the things the move can touch, step the position, keep the speed when the def asks for it, then settle on the surface and commit the new position
// Grade: DIFFER(441v441) -- frame-slot swap only: retail puts the vector<CThing*> at [esp+0x18] and the old-position C3DVector at [esp+0x24]; ours swaps them. x87 order solved (GetLength `X*X + (Y*Y + Z*Z)`; Normalise through x/y/z float locals summed y, z, x). Tried: declaration order, scoped box/position blocks, default-ctor + assignment, full STLport vector base, POD vector.
#include <math.h>
#pragma intrinsic(sqrt)
extern "C" void __cdecl free(void* block);                   // 0x00BFEA14
extern unsigned long g_InvSqrtMantissaTable[128];           // 0x013CE558
struct C3DVector {
    float X;
    float Y;
    float Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    float GetLength() const { return (float)sqrt(X * X + (Y * Y + Z * Z)); }
    __forceinline void Normalise()
    {
        float x = X;
        float y = Y;
        float z = Z;
        float lengthSquared = y * y;
        lengthSquared += z * z;
        lengthSquared += x * x;
        unsigned long bits = *(unsigned long*)&lengthSquared;
        unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) | (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
        float r = *(float*)&estimate;
        r = (3.0f - r * r * lengthSquared) * r;
        r *= 0.5f;
        X *= r;
        Y *= r;
        Z *= r;
    }
    void operator*=(const float& s) { X *= s; Y *= s; Z *= s; }
};
struct C2DBoxI { long Left; long Top; long Right; long Bottom; };
class CThing;
struct CThingPtrVector {                            // retail STLport vector<CThing*> (12 bytes)
    CThing** first;
    CThing** last;
    CThing** endOfStorage;
    CThingPtrVector() : first(0), last(0), endOfStorage(0) { }
    ~CThingPtrVector() { if (first) free(first); }
};
struct CPhysicsDef {
    unsigned char _pad00[0x3c];
    unsigned long InteractionFlags;                 // +0x3c
};
#define V4(n) virtual void v##n##a(); virtual void v##n##b(); virtual void v##n##c(); virtual void v##n##d();
struct CTCPhysicsStandard {
    V4(0) V4(1) V4(2) V4(3) V4(4) V4(5) V4(6)        // slots 0-27
    virtual void v28(); virtual void v29();
    virtual void SetPosition(const C3DVector& pos); // slot 30 (+0x78)
    virtual void v31();
    V4(8) V4(9) V4(10) V4(11) V4(12) V4(13) V4(14) V4(15) V4(16) V4(17) V4(18) V4(19) V4(20) V4(21) V4(22) V4(23)   // 32-95
    virtual void v96();
    virtual void UpdateOnSurface(CThingPtrVector& things, C3DVector& oldPosition);   // slot 97 (+0x184)
    virtual C2DBoxI GetPossibleCollidedWithArea();  // slot 98 (+0x188)
    long Owner;                                     // +0x04
    long _pad08;
    C3DVector Position;                             // +0x0c
    C3DVector OldPosition;                          // +0x18
    long OldPositionLastFrameSet;                   // +0x24
    C3DVector Velocity;                             // +0x28
    float Radius;                                   // +0x34
    CPhysicsDef* PhysicsDef;                        // +0x38
    unsigned char Moving : 1;                       // +0x3c bit 0
    void PerformForceUpdates();                     // 0x008E5670
    void GetPossibleInteractedWithList(const C2DBoxI& area, CThingPtrVector& things, float speed);   // 0x008E57D0
    void UpdateVelocityForImpassability(const C3DVector& oldPosition, C3DVector& velocity);       // 0x007243C0
    void PerformStepBasedMovement(CThingPtrVector& things, C3DVector& oldPosition);              // 0x008E5880
    void UpdateIsMoving();                          // 0x008E5270
    void Move();
};

void CTCPhysicsStandard::Move()
{
    if (!Moving)
        return;
    PerformForceUpdates();
    float speed = Velocity.GetLength();
    bool keepSpeed = (PhysicsDef->InteractionFlags >> 7) & 1;
    C2DBoxI area = GetPossibleCollidedWithArea();
    CThingPtrVector things;
    GetPossibleInteractedWithList(area, things, speed);
    C3DVector oldPosition(Position);
    if (PhysicsDef->InteractionFlags & 0x200)
        UpdateVelocityForImpassability(oldPosition, Velocity);
    PerformStepBasedMovement(things, oldPosition);
    if (Moving) {
        if (keepSpeed) {
            Velocity.Normalise();
            Velocity *= speed;
        }
        UpdateOnSurface(things, oldPosition);
        UpdateIsMoving();
        SetPosition(oldPosition);
    }
}
