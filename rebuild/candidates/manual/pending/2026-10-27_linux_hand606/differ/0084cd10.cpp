// NCameraModes::SetCameraRoll(CCamera&, float) -- rebuilds the camera's up vector for its current view direction (the perpendicular with z = 1, normalised), rolls it about the view direction by the angle, and re-applies the view
// Grade: DIFFER(194v194) -- 4 lines, scheduling: the forward Z dword is loaded/stored with X/Y (retail after the first fmul). This pass: C3DVector class key fixed (V, so Rotate/SetView mangle ABVC3DVector as the PDB), copy constructor from the member, len2 before up, named len2 +=, up 3-float ctor: no change. Lane gap: scheduling
// Leaf: ?SetCameraRoll@NCameraModes@@
// Earlier grade: DIFFER(194v194) -- forward Z load scheduled with X/Y; tried: up-before-forward order, 3-float ctor copy
// Retail: Fable.exe (TLC, VC7.1) @ 0x0084CD10, 194 bytes, __fastcall (ecx = camera, the roll angle on the stack), `ret 4`.
// Identity: Ego_r.pdb ?SetCameraRoll@NCameraModes@@YIXAAVCCamera@@M@Z; callers 0x0084DF75 and 0x0087BB77.
// Callees: C3DVector::AccurateNormalise() 0x00A14440 (in place, returns *this; manifest "C3DVector::GetScaled" is
//   propagated), C3DVector::Rotate(const C3DVector& axis, float angle) 0x00A13C60 (landed), CCamera::SetView(const
//   C3DVector& forward, const C3DVector& up) 0x00A0BE30.
// Layout: Ego_r.pdb CCamera: WorldPosition +0, RHSet (CRightHandedSet: Up +0xc, Forward +0x18), HeightLocked +0x24 ...
//   Only RHSet.Forward is read (copied into a frame local through integer registers).
// Shape: up = (?, ?, 1); the horizontal components solve dot(up, forward) = 0 by the larger horizontal axis of the
//   forward vector (`fabs` compare, `test ah,5 ; jp` = the ordered greater-than); forward.Z / (X*X + Y*Y) is computed
//   once ahead of the branch; each arm writes one component as -(k * axis) and derives the other by ratio.
extern "C" double __cdecl fabs(double x);
#pragma intrinsic(fabs)
inline float fabsf(float x) { return (float)fabs((double)x); }   // VC7.1 <math.h> C++ float overload

#pragma pack(push, 1)
class C3DVector {                                        // Ego_r.pdb: packed (CThing::Pos sits at +0xa6)
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }   // the engine's memberwise copy constructor
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector& AccurateNormalise();                      // 0x00A14440
    void Rotate(const C3DVector& axis, float angle);     // 0x00A13C60
};
#pragma pack(pop)

struct CRightHandedSet {
    C3DVector Up;                                        // +0x00
    C3DVector Forward;                                   // +0x0c
};

class CCamera {
public:
    void SetView(const C3DVector& forward, const C3DVector& up);   // 0x00A0BE30
    C3DVector       WorldPosition;                       // +0x00
    CRightHandedSet RHSet;                               // +0x0c
    bool            HeightLocked;                        // +0x24
    bool            ViewVecZLocked;                      // +0x25
    unsigned long   FOVFlags;                            // +0x28
    float           HorizontalFOV;                       // +0x2c
    float           VerticalFOV;                         // +0x30
    float           Zoom;                                // +0x34
};

namespace NCameraModes {
void __fastcall SetCameraRoll(CCamera& camera, float roll)
{
    C3DVector forward(camera.RHSet.Forward);
    C3DVector up;
    up.Z = 1.0f;
    float len2 = forward.X * forward.X + forward.Y * forward.Y;
    if (fabsf(forward.X) > fabsf(forward.Y)) {
        up.X = -(forward.Z / len2 * forward.X);
        up.Y = up.X / forward.X * forward.Y;
    } else {
        up.Y = -(forward.Z / len2 * forward.Y);
        up.X = up.Y / forward.Y * forward.X;
    }
    up.AccurateNormalise();
    up.Rotate(forward, roll);
    camera.SetView(forward, up);
}
}
