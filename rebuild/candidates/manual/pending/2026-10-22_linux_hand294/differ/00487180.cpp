// Grade: DIFFER(612v612) -- scheduling only: retail stores the forward vector's Y after the hoisted push 0 (ours before the Z multiply), and the second Normalise's table-load shl is scheduled later; tried 12 Normalise/lerp/store-order spellings
// CPlayer::GetFreeCamera(float t) const -- the free camera interpolated between last frame's and this frame's: position lerped, the right-handed set's forward and up vectors lerped and renormalised, with the current camera's horizontal FOV
// Leaf: ?GetFreeCamera@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00487180, length 612 bytes, `ret 8` (hidden CCamera result + t).
// Callees: CCamera::GetHorizontalFOV() const 0x00A0BEB0 (manifest label CMap::DrawGetSeaLevel is a byte-identical
// getter), CCamera::CCamera(const C3DVector&, const C3DVector&, const C3DVector&, float, float) 0x00A0C130.
// Globals: the reciprocal-square-root mantissa table 0x013CE558, 3.0f 0x0122DED4, 0.5f 0x0122F59C.
// Layout: engine/CPlayer.h (FreeCamera +0x78, OldFreeCamera +0xB0, CCamera 0x38 bytes; Ego_r.pdb CCamera WorldPosition +0,
// RHSet +0xc = CRightHandedSet {Up +0, Forward +0xc}).
// Shape: C3DVector inline operators `(cur - old) * t + old`, the landed inline fast Normalise (as the landed
// CTCAssassinRush::SetupFacingDirection 0x007A6E20), the camera built straight into the hidden result.
#include "engine/CPlayer.h"
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558

struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    __forceinline void Normalise()
    {
        float x = X;
        float y = Y;
        float z = Z;
        float lengthSquared = x * x + y * y + z * z;
        unsigned long bits = *(unsigned long*)&lengthSquared;
        unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                                 (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
        float r = *(float*)&estimate;
        r = (3.0f - r * r * lengthSquared) * r * 0.5f;
        X *= r;
        Y *= r;
        Z *= r;
    }
};
struct CRightHandedSet {
    C3DVector Up;                                       // +0x00
    C3DVector Forward;                                  // +0x0c
};
struct CCamera {                                        // 0x38 bytes
    C3DVector       WorldPosition;                      // +0x00
    CRightHandedSet RHSet;                              // +0x0c
    unsigned char   _body[0x38 - 0x24];
    CCamera(const C3DVector& position, const C3DVector& forward, const C3DVector& up, float fov, float zoom);   // 0x00A0C130
    float GetHorizontalFOV() const;                     // 0x00A0BEB0
};

struct CPlayer_Methods : CPlayer {
    const CCamera& FreeCam() const { return *reinterpret_cast<const CCamera*>(FreeCamera); }
    const CCamera& OldCam() const { return *reinterpret_cast<const CCamera*>(OldFreeCamera); }
    CCamera GetFreeCamera(float t) const;
};

CCamera CPlayer_Methods::GetFreeCamera(float t) const
{
    C3DVector position = (FreeCam().WorldPosition - OldCam().WorldPosition) * t + OldCam().WorldPosition;
    C3DVector forward = (FreeCam().RHSet.Forward - OldCam().RHSet.Forward) * t + OldCam().RHSet.Forward;
    forward.Normalise();
    C3DVector up = (FreeCam().RHSet.Up - OldCam().RHSet.Up) * t + OldCam().RHSet.Up;
    up.Normalise();
    return CCamera(position, forward, up, FreeCam().GetHorizontalFOV(), 0.0f);
}
