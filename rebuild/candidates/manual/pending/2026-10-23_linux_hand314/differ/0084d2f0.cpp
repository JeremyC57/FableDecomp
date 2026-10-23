// NCameraModes::CBase::IsCameraOcclusionFromTo(const C3DVector& from, const C3DVector& to, C3DVector& cameraPos) const -- within 100 units, test line of sight from `from` to `to` (0.01 radius); if blocked, cameraPos = hit + normal * radius; else (when the mode def allows it) probe four points 0.225 to each side of `to` (perpendicular and up), and if any is blocked compute the pulled-in camera position
// Grade: DIFFER(1134v1181) -- 75 blocks: frame 0x58 v 0x4c (retail overlays the probe-point and cross-product temporaries), x87 store scheduling of the cross product, and retail duplicates the GetOccludedCameraPos tail for the side and up probe pairs; branch structure, constants, call arguments and parameter-slot reuse (radius, side-probe flag) line up
// Leaf: IsCameraOcclusionFromTo
// Retail 0x0084D2F0, 1184 bytes, `ret 0xc`.
// Callees: C3DVector::Normalise 0x00A14440; CTools::IsLineOfSightFiltered 0x00877C40; the occluded-camera-position helper
//   0x0084CCC0 (`__fastcall`, returns C3DVector).
// Layout: Ego_r.pdb NCameraModes::CBase less 0x10 (retail CPhysicsState LookAt is 0x30): CameraManager +0x40, PCameraModeDef
//   +0x44; the camera manager's CTools at +0x78 and the mode def's side-probe flag at +0x74 from this function's use.
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
    void Normalise();                                  // 0x00A14440
};
inline C3DVector CrossProduct(const C3DVector& a, const C3DVector& b)
{
    return C3DVector(a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - a.Y * b.X);
}
extern "C" double __cdecl fabs(double v);
#pragma intrinsic(fabs)
inline float Abs(float v) { return (float)fabs(v); }
class CThing;
struct CTools {
    bool IsLineOfSightFiltered(const C3DVector& from, const C3DVector& to, C3DVector* hitPos, C3DVector* hitNormal,
                               float* radius, CThing** hitThing, unsigned long flags, bool ignoreTransparent) const;   // 0x00877C40
};
struct CGameCameraManager {
    unsigned char _pad00[0x78];
    CTools        Tools;                               // +0x78
};
struct CCameraModeDef {
    unsigned char _pad00[0x74];
    bool          SideProbes;                          // +0x74
};
namespace NCameraModes {
class CBase {
public:
    unsigned char       _pad00[0x40];
    CGameCameraManager* CameraManager;                 // +0x40
    CCameraModeDef*     PCameraModeDef;                // +0x44
    static C3DVector __fastcall GetOccludedCameraPos(const C3DVector& hitPos, const C3DVector& hitNormal, float radius,
                                                     const C3DVector& direction);   // 0x0084CCC0
    bool IsCameraOcclusionFromTo(const C3DVector& from, const C3DVector& to, C3DVector& cameraPos) const;
};
}

bool NCameraModes::CBase::IsCameraOcclusionFromTo(const C3DVector& from, const C3DVector& to, C3DVector& cameraPos) const
{
    if ((to - from).GetLengthSquared() > 10000.0f)
        return false;
    bool occluded = false;
    float radius = 0.01f;
    C3DVector side;
    side.Z = 0.0f;
    bool sideProbes = PCameraModeDef->SideProbes;
    C3DVector direction = from - to;
    C3DVector up;
    if (Abs(direction.Y) > 0.0001f) {
        side.X = 1.0f;
        side.Y = -(direction.X / direction.Y);
        side.Normalise();
        up = CrossProduct(direction, side);
        up.Normalise();
    } else if (Abs(direction.X) > 0.0001f) {
        side.Y = 1.0f;
        side.X = -(direction.Y / direction.X);
        side.Normalise();
        up = CrossProduct(direction, side);
        up.Normalise();
    } else {
        sideProbes = false;
    }
    CThing* hitThing = 0;
    C3DVector hitPos;
    C3DVector hitNormal;
    if (!CameraManager->Tools.IsLineOfSightFiltered(from, to, &hitPos, &hitNormal, &radius, &hitThing, 15, true)) {
        occluded = true;
        cameraPos = hitPos + hitNormal * radius;
        return occluded;
    }
    if (sideProbes) {
        if (!CameraManager->Tools.IsLineOfSightFiltered(to, to + side * 0.225f, &hitPos, &hitNormal, &radius, &hitThing, 9, true) ||
            !CameraManager->Tools.IsLineOfSightFiltered(to, to - side * 0.225f, &hitPos, &hitNormal, &radius, &hitThing, 9, true) ||
            !CameraManager->Tools.IsLineOfSightFiltered(to, to + up * 0.225f, &hitPos, &hitNormal, &radius, &hitThing, 9, true) ||
            !CameraManager->Tools.IsLineOfSightFiltered(to, to - up * 0.225f, &hitPos, &hitNormal, &radius, &hitThing, 9, true)) {
            cameraPos = GetOccludedCameraPos(hitPos, hitNormal, radius, direction);
            occluded = true;
        }
    }
    return occluded;
}
