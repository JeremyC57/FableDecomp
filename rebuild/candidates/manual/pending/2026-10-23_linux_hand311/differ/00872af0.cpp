// Grade: DIFFER(935v971) -- heavy x87 scheduling: retail computes sin then the YAxis products before fcos, keeps camera.Y on the x87 stack into the clamp block, and parks the int sign in the projection's own slot; two strategies (named s/c floats, inline sin/cos in the expression) did not converge
// NCameraModes::CZTargetNew::GetCameraPosition() -- orbit the camera around the observed thing (theta, height and XY offset blended during a transition); outside a transition and while the target lives, let the camera trail its goal only within the cage box along the target axes and keep it at least the cage radius from the thing
// Leaf: ?GetCameraPosition@CZTargetNew@NCameraModes@@QAE?AVC3DVector@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00872AF0, 971 bytes (+ int3 padding), ret 4 (C3DVector by hidden pointer).
// Callees: NCameraModes::CBase::GetObservedThing() const 0x0084F1B0, CThing::GetPosition 0x004C73D0 (const C3DVector&),
//   CBaseIntelligentPointer::GetPItem 0x00A01B50 (twice); 2*pi as double @ 0x0124F2B8, 0.0f @ 0x0122DEDC, 1.0f @ 0x0122DED8.
// Layout (retail NCameraModes::CBase is 0xA4 bytes, PDB 0xC8, as the landed Update 0x00873030): GoTo position +0x54,
//   PCameraModeDef +0x44 (cage half-extents at def +0x6C / +0x70); CZTargetNew PTargetedThing +0xA4, BaseOffsetXY +0xAC,
//   BaseHeightOffset +0xB0, Theta +0xB4, DeltaBaseOffsetXY +0xB8, DeltaBaseHeightOffset +0xBC, DeltaTheta +0xC0,
//   TargetBaseOffsetXY +0xC4, TargetBaseHeightOffset +0xC8, StringLengthXY +0xCC, CageRadius +0xD0, XAxis +0xD4,
//   YAxis +0xE0, FramesForTransition +0xEC. CThing dead bit +0x91.
#include <math.h>
#pragma intrinsic(sin, cos, sqrt, fabs)
#define PI_F 3.1415927f
struct C2DVector { float X, Y; };
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
};
static inline float DotProduct(const C3DVector& a, const C3DVector& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }
static inline long Sign(float f) { return f < 0.0f ? -1 : 1; }

struct CThing {
    unsigned char _pad00[0x91];
    unsigned char Dead : 1;                             // +0x91 bit 0
    const C3DVector& GetPosition() const;               // 0x004C73D0
};
struct CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;
    CBaseObject* GetPItem() const;                      // 0x00A01B50
};
struct CCameraModeDef {
    unsigned char _pad[0x6c];
    C2DVector ZTargetCage;                              // +0x6C
};
struct CPhysicsState {
    C3DVector Position;
    unsigned char _rest[0x24];
};
namespace NCameraModes {
class CBase {
public:
    virtual void v00();
    float DefaultFOV;                                   // +0x04
    float DefaultRollAngle;                             // +0x08
    float DefaultObservedPositionZOffset;               // +0x0c
    CPhysicsState LookAt;                               // +0x10
    void* CameraManager;                                // +0x40
    const CCameraModeDef* PCameraModeDef;               // +0x44
    void* PCameraModeDefInfo_SubComponents[3];          // +0x48
    CPhysicsState GoTo;                                 // +0x54
    unsigned char _pad84[0x20];
    const CThing* GetObservedThing() const;             // 0x0084F1B0
};
class CZTargetNew : public CBase {
public:
    CBaseIntelligentPointer PTargetedThing;             // +0xA4
    float BaseOffsetXY;                                 // +0xAC
    float BaseHeightOffset;                             // +0xB0
    float Theta;                                        // +0xB4
    float DeltaBaseOffsetXY;                            // +0xB8
    float DeltaBaseHeightOffset;                        // +0xBC
    float DeltaTheta;                                   // +0xC0
    float TargetBaseOffsetXY;                           // +0xC4
    float TargetBaseHeightOffset;                       // +0xC8
    float StringLengthXY;                               // +0xCC
    float CageRadius;                                   // +0xD0
    C3DVector XAxis;                                    // +0xD4
    C3DVector YAxis;                                    // +0xE0
    long FramesForTransition;                           // +0xEC
    C3DVector GetCameraPosition();
};
}

C3DVector NCameraModes::CZTargetNew::GetCameraPosition()
{
    C3DVector targetPos = GetObservedThing()->GetPosition();
    bool free = false;
    if (FramesForTransition > 0)
    {
        --FramesForTransition;
        free = true;
        Theta += DeltaTheta;
        BaseHeightOffset += DeltaBaseHeightOffset;
        BaseOffsetXY += DeltaBaseOffsetXY;
    }
    if (!PTargetedThing.GetPItem() || ((const CThing*)PTargetedThing.GetPItem())->Dead)
        free = true;
    C3DVector centre(targetPos.X, targetPos.Y, BaseHeightOffset + targetPos.Z);
    double angle = Theta * (2.0 * PI_F);
    float s = (float)sin(angle);
    float c = (float)cos(angle);
    C3DVector camera = (XAxis * c + YAxis * s) * BaseOffsetXY + centre;
    if (free)
        return camera;

    C2DVector cage = PCameraModeDef->ZTargetCage;
    C3DVector delta(GoTo.Position.X - camera.X, GoTo.Position.Y - camera.Y, 0.0f);
    float alongX = DotProduct(delta, XAxis);
    float alongY = DotProduct(delta, YAxis);
    if (fabs(alongX) > cage.X)
        alongX = (float)Sign(alongX) * cage.X;
    if (fabs(alongY) > cage.Y)
        alongY = (float)Sign(alongY) * cage.Y;
    C3DVector position = camera + XAxis * alongX + YAxis * alongY;
    C3DVector fromTarget = position - targetPos;
    float lengthSquared = fromTarget.Y * fromTarget.Y + fromTarget.X * fromTarget.X;
    if (lengthSquared < CageRadius * CageRadius)
    {
        float inv = 1.0f / (float)sqrt(lengthSquared);
        position = C3DVector(fromTarget.X, fromTarget.Y, 0.0f) * CageRadius * inv + centre;
    }
    return position;
}
