// CBulletTimeManager::ConvertInterpolateToBulletTimeInterpolate(float interpolate) const -- rescale an interpolation step by the bullet-time ratio, carrying the frame-count remainder
// Leaf: ?ConvertInterpolateToBulletTimeInterpolate@CBulletTimeManager@@QBEMM@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C6010, length 78 bytes (+ int3 padding).
// Name (agentNF3, 2026-11-10): Ego_r.pdb ?ConvertInterpolateToBulletTimeInterpolate@CBulletTimeManager@@QBEMM@Z (const member;
//   engine/CBulletTimeManager.h FABLE_RETAIL_CBULLETTIMEMANAGER_INTERPOLATE declares it; the landed 0x007EA2F0
//   NTargeting::GetTargetingPositionOnThing binds this spelling). Was landed as a non-const CBulletTime:: member (descriptive class).
// Callees: CBulletTimeManager::GetBulletTimeSlowdownMultiplier() const @ 0x004C5FA0 (float, on `this`: ecx passes straight through;
//   Ego_r.pdb / FableWin.pdb, landed spelling of 0x006655A0), CWorld::GetFrame @ 0x0049D870 (returns the global @ 0x013B89BC),
//   __ftol2 @ 0x00BFEA70. Globals: the float constant @ 0x0122DED8 (the numerator of the ratio).
// Shape: two frame slots. `scale` (slot 0) is live across the frame-count call and is only truncated after it; `ratio` = K / scale
// (slot 1). `(frames - 1) % (long)scale` is formed with `cdq ; idiv`; the remainder is then spilled into slot 0 -- `scale`'s slot,
// free because `scale` is in a nested scope -- for the `fild`. The result `ratio * rem + interpolate * ratio` stays in st(0):
// `fild ; fmul [ratio] ; fld [ratio] ; fmul [arg] ; faddp`.
#define FABLE_RETAIL_STLP_MAP_EMPTY
#define FABLE_RETAIL_STLP_SET_EMPTY
#define FABLE_RETAIL_CBULLETTIMEMANAGER
#define FABLE_RETAIL_CBULLETTIMEMANAGER_INTERPOLATE
#define FABLE_RETAIL_CBULLETTIMEMANAGER_SLOWDOWNMULTIPLIER    // hdr overlay: GetBulletTimeSlowdownMultiplier 0x004C5FA0
#include "engine/CBulletTimeManager.h"
class CWorld { public: static long __fastcall GetFrame(); };                         // 0x0049D870 ?GetFrame@CWorld@@SIJXZ

extern const float g_bulletTimeRatioNumerator;  // 0x0122DED8

float CBulletTimeManager::ConvertInterpolateToBulletTimeInterpolate(float interpolate) const
{
    float ratio;
    long  divisor;
    long  frames;
    {
        float scale = GetBulletTimeSlowdownMultiplier();
        ratio = g_bulletTimeRatioNumerator / scale;
        frames = CWorld::GetFrame();
        divisor = (long)scale;
    }
    long rem = (frames - 1) % divisor;
    float carried = ratio * (float)rem;
    float stepped = interpolate * ratio;
    return carried + stepped;
}
