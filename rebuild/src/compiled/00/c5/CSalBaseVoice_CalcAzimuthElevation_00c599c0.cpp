// CSalBaseVoice::CalcAzimuthElevation(float& azimuth, float& elevation, const CCodaVector& direction) -- GameCODA software voice: convert a listener-relative direction into degrees: elevation = the angle of y over the horizontal length (signed by y), azimuth = the angle of x around z (0 ahead, 90 to the side, 180 behind, negated for x < 0, -180 folded to 180); an output in mode 2 forces azimuth 0
// Leaf: ?CalcAzimuthElevation@CSalBaseVoice@@KAXAAM0ABVCCodaVector@@@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C599C0, 318 bytes, __cdecl (`ret`): (azimuth, elevation, direction) on the stack.
//   Float pool: 0.0f 0x0122DEDC, 57.29578f (180/pi) 0x012B8F70, 90.0f 0x012B7B74, 180.0f 0x012783A0, -180.0f 0x012B7B64;
//   the -180 fold stores 180.0f as the immediate 0x43340000.
// Callers: the software voice's parameter update 0x00C54C10 (call at 0x00C54C71) passes (&voice+0x210, &voice+0x214,
//   &params+0x120) when bit 15 of its change mask is set.
// Callees: CCodaSystem::GetOutputMode 0x00C3EA60 on g_pCodaSystem 0x0143BC2C (the spellings the landed same-module
//   ComputePanGains 0x00C59960 binds; mode 2 also cancels the pan there).
// Name (NF93 q11407): GameCODA 2.10 SDK CodeView (CODAApi.lib / CODAApiDbg.lib / OpenAL32.lib) ?CalcAzimuthElevation@CSalBaseVoice@@KAXAAM0ABVCCodaVector@@@Z,
//   the protected static of engine/CSalBaseVoice.h (FABLE_RETAIL_CSALSW3D_CALCPARAMS); the landed CalcParams 0x00C54C10 already binds it.
//   Was the descriptive free ComputeAzimuthElevation(float*, float*, const C3DVector*) (the manifest's "Audio_ComputePitchFromVector" was wrong).
// Shape: atan2 under /O2 is the `fpatan` intrinsic and sqrt the `fsqrt` intrinsic; the |y| / |x| / |z| are the
//   `v < 0 ? -v : v` form (compare, reload, `fchs`); the elevation arm has no zero test, the azimuth arm has one; the azimuth test is `z > x` (`fcom st(1) ; test ah,41h ; jne`), the elevation test `y < horizontal` (`fld st(1) ; fcomp st(1) ; test ah,5 ; jp`).
#include <math.h>
#define FABLE_RETAIL_PASS22_CSALSW_CALCPARAMS
#define FABLE_RETAIL_PASS22_CCODA3D_CREATE
#define FABLE_RETAIL_CSALSW3D_CALCPARAMS
#include "engine/CSalSw3DVoice.h"     // CSalBaseVoice (CSALSW3D_CALCPARAMS: CalcAzimuthElevation) and CCodaVector, as the landed 0x00C54C10
#include "engine/CCodaVector.h"
class CCodaSystem {
public:
    long GetOutputMode();               // 0x00C3EA60
};
extern CCodaSystem* g_pCodaSystem;      // 0x0143BC2C

void FABLE_CDECL CSalBaseVoice::CalcAzimuthElevation(float& azimuth, float& elevation, const CCodaVector& direction)
{
    float y = direction.y < 0.0f ? -direction.y : direction.y;
    float horizontal = (float)sqrt(direction.x * direction.x + direction.z * direction.z);
    if (y < horizontal)
        elevation = (float)atan2(y, horizontal) * 57.29578f;
    else
        elevation = 90.0f - (float)atan2(horizontal, y) * 57.29578f;
    if (direction.y < 0.0f)
        elevation = -elevation;

    float x = direction.x < 0.0f ? -direction.x : direction.x;
    float z = direction.z < 0.0f ? -direction.z : direction.z;
    if (z > x)
        azimuth = (float)atan2(x, z) * 57.29578f;
    else if (x == 0.0f)
        azimuth = 0.0f;
    else
        azimuth = 90.0f - (float)atan2(z, x) * 57.29578f;
    if (direction.z < 0.0f)
        azimuth = 180.0f - azimuth;
    if (direction.x < 0.0f)
        azimuth = -azimuth;
    if (azimuth <= -180.0f)
        azimuth = 180.0f;
    if (g_pCodaSystem->GetOutputMode() == 2)
        azimuth = 0.0f;
}