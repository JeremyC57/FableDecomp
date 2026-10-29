// CBulletTime::ConvertInterpolateToBulletTimeInterpolate(float interpolate) -- rescale an interpolation step by the bullet-time ratio, carrying the frame-count remainder
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C6010, length 78 bytes (+ int3 padding).
// Callees: CBulletTime::GetTimeScale() @ 0x004C5FA0 (float, on `this`: ecx passes straight through),
// CWorld::GetFrame @ 0x0049D870 (returns the global @ 0x013B89BC), __ftol2 @ 0x00BFEA70.
// Globals: the float constant @ 0x0122DED8 (the numerator of the ratio).
// Shape: two frame slots. `scale` (slot 0) is live across the frame-count call and is only
// truncated after it; `ratio` = K / scale (slot 1). `(frames - 1) % (long)scale` is formed with
// `cdq ; idiv`; the remainder is then spilled into slot 0 -- `scale`'s slot, free because `scale`
// is in a nested scope -- for the `fild`. The result `ratio * rem + interpolate * ratio` stays in
// st(0): `fild ; fmul [ratio] ; fld [ratio] ; fmul [arg] ; faddp`.
// The manifest's "CBulletTimeManager" is the bullet-time object (+0x04 controller, +0x0c/+0x0d
// flags, +0x10 multiplier) also used by SetBulletTimeSlowdownMultiplier @ 0x004C6170.
class CWorld { public: static long __fastcall GetFrame(); };                         // 0x0049D870

extern const float g_bulletTimeRatioNumerator;  // 0x0122DED8

struct CBulletTime {
    float GetTimeScale();                       // 0x004C5FA0
    float ConvertInterpolateToBulletTimeInterpolate(float interpolate);
};

float CBulletTime::ConvertInterpolateToBulletTimeInterpolate(float interpolate)
{
    float ratio;
    long  divisor;
    long  frames;
    {
        float scale = GetTimeScale();
        ratio = g_bulletTimeRatioNumerator / scale;
        frames = CWorld::GetFrame();
        divisor = (long)scale;
    }
    long rem = (frames - 1) % divisor;
    float carried = ratio * (float)rem;
    float stepped = interpolate * ratio;
    return carried + stepped;
}