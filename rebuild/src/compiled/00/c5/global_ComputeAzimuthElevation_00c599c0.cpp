// ComputeAzimuthElevation(float* azimuth, float* elevation, const C3DVector* direction) -- GameCODA software voice: convert a listener-relative direction into degrees: elevation = the angle of y over the horizontal length (signed by y), azimuth = the angle of x around z (0 ahead, 90 to the side, 180 behind, negated for x < 0, -180 folded to 180); an output in mode 2 forces azimuth 0
// Leaf: ?ComputeAzimuthElevation@@YAXPAM0PBVC3DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C599C0, 318 bytes, __cdecl (`ret`): (azimuth, elevation, direction) on the stack.
//   Float pool: 0.0f 0x0122DEDC, 57.29578f (180/pi) 0x012B8F70, 90.0f 0x012B7B74, 180.0f 0x012783A0, -180.0f 0x012B7B64;
//   the -180 fold stores 180.0f as the immediate 0x43340000.
// Callers: the software voice's parameter update 0x00C54C10 (call at 0x00C54C71) passes (&voice+0x210, &voice+0x214,
//   &params+0x120) when bit 15 of its change mask is set.
// Callees: CCodaSystem::GetOutputMode 0x00C3EA60 on g_pCodaSystem 0x0143BC2C (the spellings the landed same-module
//   ComputePanGains 0x00C59960 binds; mode 2 also cancels the pan there).
// Name: descriptive (GameCODA has no PDB; the manifest's "Audio_ComputePitchFromVector" is descriptive and wrong: no pitch).
// Shape: atan2 under /O2 is the `fpatan` intrinsic and sqrt the `fsqrt` intrinsic; the |y| / |x| / |z| are the
//   `v < 0 ? -v : v` form (compare, reload, `fchs`); the elevation arm has no zero test, the azimuth arm has one; the azimuth test is `z > x` (`fcom st(1) ; test ah,41h ; jne`), the elevation test `y < horizontal` (`fld st(1) ; fcomp st(1) ; test ah,5 ; jp`).
#include <math.h>
class C3DVector { public:
    float X, Y, Z;
};
class CCodaSystem {
public:
    long GetOutputMode();               // 0x00C3EA60
};
extern CCodaSystem* g_pCodaSystem;      // 0x0143BC2C

void __cdecl ComputeAzimuthElevation(float* azimuth, float* elevation, const C3DVector* direction)
{
    float y = direction->Y < 0.0f ? -direction->Y : direction->Y;
    float horizontal = (float)sqrt(direction->X * direction->X + direction->Z * direction->Z);
    if (y < horizontal)
        *elevation = (float)atan2(y, horizontal) * 57.29578f;
    else
        *elevation = 90.0f - (float)atan2(horizontal, y) * 57.29578f;
    if (direction->Y < 0.0f)
        *elevation = -*elevation;

    float x = direction->X < 0.0f ? -direction->X : direction->X;
    float z = direction->Z < 0.0f ? -direction->Z : direction->Z;
    if (z > x)
        *azimuth = (float)atan2(x, z) * 57.29578f;
    else if (x == 0.0f)
        *azimuth = 0.0f;
    else
        *azimuth = 90.0f - (float)atan2(z, x) * 57.29578f;
    if (direction->Z < 0.0f)
        *azimuth = 180.0f - *azimuth;
    if (direction->X < 0.0f)
        *azimuth = -*azimuth;
    if (*azimuth <= -180.0f)
        *azimuth = 180.0f;
    if (g_pCodaSystem->GetOutputMode() == 2)
        *azimuth = 0.0f;
}