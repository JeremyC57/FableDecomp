// CCamera::SetAngleYZ(float angleYZ) -- set the camera's pitch (in turns): rebuild the forward vector from the current heading (GetAngleXY) and the new pitch, store the up vector as (heading of the old up, angleYZ, 1), then normalise both
// Grade: DIFFER(318v315) -- two allocator choices: ours CSEs &RHSet.Forward into edi at entry (`push edi ; lea edi,[esi+18h]`, reused for the final copy and Normalise) where retail recomputes `lea eax/ecx,[esi+0Ch]` off the advanced esi; and the forward products are `fmul st(2)` / `fmul st(1)` + `fstp st(0)` where retail copies cos(yz) (`fld st(2) ; fmulp st(1)`) and consumes it with `fmulp st(1)`. Operand order, named float/double locals, CSE and the QFE-4035 compiler all give ours
// Leaf: ?SetAngleYZ@CCamera@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0C6F0, 313 bytes, thiscall `ret 4`. No direct callers in the image.
// Identity: Ego_r.pdb `?SetAngleYZ@CCamera@@QAEXM@Z` (CCamera's angle accessors run GetAngleXY 0x00A0C380, GetAngleYZ
//   0x00A0C3E0, GetAngleXZ 0x00A0C450, SetAngleXY 0x00A0C570, SetAngleYZ here, SetAngleXZ 0x00A0C830 in both images; the chunk
//   label "Math_SetRotationFromAngles" is descriptive).
// Layout: Ego_r.pdb CCamera (0x38): WorldPosition +0x00, RHSet +0x0C (Up +0x0C, Forward +0x18).
// Callees: _CIfmod (thunk 0x00BFEB28), C3DVector::Normalise @ 0x00A14440 (twice); atan2 / sin / cos are x87 intrinsics.
// Globals (pool): 1/(2pi) 0x01237F08, 0.0f 0x0122DEDC, 1.0f 0x0122DED8, 1.0 (double) 0x01237F00, 2pi (double) 0x0124F2B8.
// Shape: the angle helpers (atan2 over 2pi wrapped into [0, 1) by fmod(x, 1.0) plus one when negative) are expanded in
//   place; the forward vector is (sin(xy) cos(yz), cos(xy) cos(yz), sin(yz)) with cos(yz) computed first, held in a local and
//   assigned after the up vector; the up vector really is the temporary C3DVector(old-up heading, angleYZ, 1.0f) (retail
//   stores no trig there).
typedef unsigned short wchar_t;                     // VC7.1 default /Zc:wchar_t-
extern "C" double __cdecl atan2(double y, double x);
extern "C" double __cdecl fmod(double x, double y);
extern "C" double __cdecl sin(double x);
extern "C" double __cdecl cos(double x);
#pragma intrinsic(atan2, fmod, sin, cos)

const double TWO_PI = 6.2831854820251465;           // (double)(2 * 3.1415927f)

struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector& Normalise();                         // 0x00A14440
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

__forceinline float GetVectorAngleXY(const C3DVector& v)
{
    return NormaliseTurns((float)atan2(v.X, v.Y) * 0.15915494f);
}

__forceinline C3DVector GetVectorFromAngles(float angleXY, float angleYZ)
{
    return C3DVector((float)(cos(angleYZ * TWO_PI) * sin(angleXY * TWO_PI)),
                     (float)(cos(angleYZ * TWO_PI) * cos(angleXY * TWO_PI)),
                     (float)sin(angleYZ * TWO_PI));
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

    __forceinline float GetAngleXY() const          // out of line @ 0x00A0C380
    {
        return NormaliseTurns((float)atan2(RHSet.Forward.X, RHSet.Forward.Y) * 0.15915494f);
    }
    void SetAngleYZ(float angleYZ);
};

void CCamera::SetAngleYZ(float angleYZ)
{
    float angleXY = GetAngleXY();
    double yz = angleYZ * TWO_PI;
    double cosYZ = cos(yz);
    double xy = angleXY * TWO_PI;
    C3DVector forward((float)(cosYZ * sin(xy)), (float)(cosYZ * cos(xy)), (float)sin(yz));
    RHSet.Up = C3DVector(GetVectorAngleXY(RHSet.Up), angleYZ, 1.0f);
    RHSet.Forward = forward;
    RHSet.Up.Normalise();
    RHSet.Forward.Normalise();
}
