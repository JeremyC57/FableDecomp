// GainToMillibels(float gain) -- linear amplitude to millibels: 2000 * log10(gain), the gain floored at 1e-5 (-10000 mB)
// Leaf: ?GainToMillibels@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C62FB0, 53 bytes (`__cdecl`, float in st0).
// Callees: none (`log` is the /Oi intrinsic: `fldln2; fxch; fyl2x`); constants 1e-5f (0x012B9410) and
//   868.589f = 2000 / ln 10 (0x012B940C).
// Identity: the manifest's "ScaleValueByConstant" is a propagated label. 1e-5 maps to exactly -10000 mB, DirectSound's
//   DSBVOLUME_MIN and EAX's room/reflection floor; the callers 0x00C6788C / 0x00C67905 convert per-source gains. Descriptive name.
// Shape: the clamp in a float local through `?:` (the compiler splits the two log paths and the floor loads as a float
//   constant); `if (gain <= m) gain = m;` merges them (42 bytes) and `return log(1e-5f) * k` folds a double constant.
extern "C" double __cdecl log(double x);
#pragma intrinsic(log)

float GainToMillibels(float gain)
{
    float clamped = gain > 1e-5f ? gain : 1e-5f;
    return (float)log(clamped) * 868.589f;
}