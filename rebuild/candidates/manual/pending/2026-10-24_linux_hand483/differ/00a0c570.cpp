// CCamera::SetAngleXY(float angleXY) -- set the camera's heading (in turns): rebuild the forward vector from the new heading and the current pitch (GetAngleYZ), rebuild the up vector from the new heading and the old up vector's own pitch, then normalise both
// Grade: DIFFER(380v373) -- same family as SetAngleYZ 0x00A0C6F0: ours CSEs &RHSet.Forward into edi at entry (`push edi ; lea edi,[esi+18h]`) and multiplies the spilled sin/cos(xy) doubles after both are computed (`fld qword ; fmul st(1)` + a trailing `fstp st(0)`); retail forms each forward product as soon as its factor exists (`fld st(1) ; fmul qword [sin]`, then `fmul qword [cos]` consuming cos(yz)). Next: CSE-only spelling (no named sin/cos locals) once the SetAngleYZ product form is solved
// Leaf: ?SetAngleXY@CCamera@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0C570, 371 bytes, thiscall `ret 4`. Caller: CPlayer::UpdateFreeCamera
//   (0x00488602).
// Identity: Ego_r.pdb `?SetAngleXY@CCamera@@QAEXM@Z` (CCamera's angle accessors: GetAngleXY 0x00A0C380, GetAngleYZ 0x00A0C3E0,
//   GetAngleXZ 0x00A0C450, SetAngleXY here, SetAngleYZ 0x00A0C6F0; the chunk label "FreeCamera_UpdateRotation" is descriptive).
// Layout: Ego_r.pdb CCamera (0x38): WorldPosition +0x00, RHSet +0x0C (Up +0x0C, Forward +0x18).
// Callees: _CIfmod (thunk 0x00BFEB28), C3DVector::Normalise @ 0x00A14440 (twice); atan2 / sqrt / sin / cos are x87 intrinsics.
// Globals (pool): 1/(2pi) 0x01237F08, 0.0f 0x0122DEDC, 1.0f 0x0122DED8, 1.0 (double) 0x01237F00, 2pi (double) 0x0124F2B8.
// Shape: the pitch helpers (atan2(z, |xy|) over 2pi wrapped into [0, 1)) are expanded in place; sin and cos of the new
//   heading are double locals (spilled as qwords across the second fmod call) shared by both vectors; each vector is
//   (sin(xy) cos(yz), cos(xy) cos(yz), sin(yz)); the old up's pitch lives in the dead angleXY slot.
typedef unsigned short wchar_t;                     // VC7.1 default /Zc:wchar_t-
extern "C" double __cdecl atan2(double y, double x);
extern "C" double __cdecl fmod(double x, double y);
extern "C" double __cdecl sin(double x);
extern "C" double __cdecl cos(double x);
extern "C" double __cdecl sqrt(double x);
#pragma intrinsic(atan2, fmod, sqrt, sin, cos)

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

__forceinline float GetVectorAngleYZ(const C3DVector& v)
{
    float length = (float)sqrt(v.X * v.X + v.Y * v.Y);
    return NormaliseTurns((float)atan2(v.Z, length) * 0.15915494f);
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

    __forceinline float GetAngleYZ() const          // out of line @ 0x00A0C3E0
    {
        float length = (float)sqrt(RHSet.Forward.X * RHSet.Forward.X + RHSet.Forward.Y * RHSet.Forward.Y);
        return NormaliseTurns((float)atan2(RHSet.Forward.Z, length) * 0.15915494f);
    }
    void SetAngleXY(float angleXY);
};

void CCamera::SetAngleXY(float angleXY)
{
    double yz = GetAngleYZ() * TWO_PI;
    double cosYZ = cos(yz);
    double xy = angleXY * TWO_PI;
    double sinXY = sin(xy);
    double cosXY = cos(xy);
    C3DVector forward((float)(sinXY * cosYZ), (float)(cosXY * cosYZ), (float)sin(yz));
    double upYZ = GetVectorAngleYZ(RHSet.Up) * TWO_PI;
    double cosUpYZ = cos(upYZ);
    RHSet.Up = C3DVector((float)(sinXY * cosUpYZ), (float)(cosXY * cosUpYZ), (float)sin(upYZ));
    RHSet.Forward = forward;
    RHSet.Up.Normalise();
    RHSet.Forward.Normalise();
}
