// CCamera::GetAngleXZ() const -- the camera's roll in turns: rebuild the up vector an unrolled camera would have from its heading (GetAngleXY + 1/2) and pitch (1/4 - GetAngleYZ), and return the angle between it and the actual up vector (acos of the dot product / 2pi)
// Grade: DIFFER(288v288) -- x87 stack choice in the two cos(pitch) products only: retail copies cos(pitch) (`fld st(2) ; fmulp st(1)`) for X and consumes it with `fmulp st(1)` for Y; ours multiplies in place (`fmul st(2)`, `fmul st(1)`) and pops cos(pitch) afterwards (`fstp st(0)`, +2 bytes net 0). The GetAngleXY / GetAngleYZ expansions, the dot product and the frame match
// Leaf: ?GetAngleXZ@CCamera@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0C450, 288 bytes, thiscall, plain `ret`, float in st(0). No direct callers.
// Identity: Ego_r.pdb `?GetAngleXZ@CCamera@@QBEMXZ` (after GetAngleXY 0x00A0C380 and GetAngleYZ 0x00A0C3E0 in both images; the
//   chunk label "Math_ComputeSphericalCoordinate" is descriptive).
// Layout: Ego_r.pdb CCamera (0x38): WorldPosition +0x00, RHSet +0x0C (Up +0x0C, Forward +0x18), HeightLocked +0x24,
//   ViewVecZLocked +0x25, FOVFlags +0x28, HorizontalFOV +0x2C, VerticalFOV +0x30, Zoom +0x34.
// Callees: _CIfmod (thunk 0x00BFEB28), _CIacos (thunk 0x00BFEBA2); atan2 / sqrt / sin / cos are x87 intrinsics.
// Globals (pool): 1/(2pi) 0x01237F08, 0.0f 0x0122DEDC, 1.0f 0x0122DED8, 1.0 (double) 0x01237F00, 0.5f 0x0122F59C,
//   0.25f 0x0123078C, 2pi (double) 0x0124F2B8.
// Shape: GetAngleXY / GetAngleYZ are expanded in place (atan2 of the forward vector over 2pi, wrapped into [0, 1) by
//   fmod(x, 1.0) plus one when negative); the heading + 1/2 is the only float local, the rebuilt up vector is a C3DVector in
//   memory with z kept in st(0) for the z, y, x dot product.
typedef unsigned short wchar_t;                     // VC7.1 default /Zc:wchar_t-
extern "C" double __cdecl atan2(double y, double x);
extern "C" double __cdecl fmod(double x, double y);
extern "C" double __cdecl acos(double x);
extern "C" double __cdecl sqrt(double x);
extern "C" double __cdecl sin(double x);
extern "C" double __cdecl cos(double x);
#pragma intrinsic(atan2, fmod, acos, sqrt, sin, cos)

const double TWO_PI = 6.2831854820251465;           // (double)(2 * 3.1415927f)

struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    float DotProduct(const C3DVector& other) const { return Z * other.Z + Y * other.Y + X * other.X; }
};

struct CRightHandedSet {
    C3DVector Up;                                   // +0x00
    C3DVector Forward;                              // +0x0C
};

__forceinline float NormaliseTurns(float angle)
{
    if (angle < 0.0f || angle >= 1.0f) {
        angle = (float)fmod(angle, 1.0);
        if (angle < 0.0f)
            angle += 1.0f;
    }
    return angle;
}

class CCamera {
public:
    C3DVector       WorldPosition;                  // +0x00
    CRightHandedSet RHSet;                          // +0x0C
    bool            HeightLocked;                   // +0x24
    bool            ViewVecZLocked;                 // +0x25
    unsigned long   FOVFlags;                       // +0x28
    float           HorizontalFOV;                  // +0x2C
    float           VerticalFOV;                    // +0x30
    float           Zoom;                           // +0x34

    __forceinline float GetAngleXY() const                 // out of line @ 0x00A0C380
    {
        return NormaliseTurns((float)atan2(RHSet.Forward.X, RHSet.Forward.Y) * 0.15915494f);
    }
    __forceinline float GetAngleYZ() const                 // out of line @ 0x00A0C3E0
    {
        float length = (float)sqrt(RHSet.Forward.X * RHSet.Forward.X + RHSet.Forward.Y * RHSet.Forward.Y);
        return NormaliseTurns((float)atan2(RHSet.Forward.Z, length) * 0.15915494f);
    }
    float GetAngleXZ() const;
};

float CCamera::GetAngleXZ() const
{
    float heading = GetAngleXY() + 0.5f;
    float pitch = 0.25f - GetAngleYZ();
    C3DVector up((float)(cos(pitch * TWO_PI) * sin(heading * TWO_PI)),
                 (float)(cos(pitch * TWO_PI) * cos(heading * TWO_PI)),
                 (float)sin(pitch * TWO_PI));
    return (float)acos(up.DotProduct(RHSet.Up)) * 0.15915494f;
}
