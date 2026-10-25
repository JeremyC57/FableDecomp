// ExponentialDecayStep(float from, float seconds) -- the per-block multiplier of an exponential envelope decay: exp(ln(32768) * (-1 / blocks)), so the level falls by 32768:1 (about 90 dB) over the decay length; 0 when the length rounds to zero blocks
// Leaf: ?ExponentialDecayStep@@YAMMM@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C59D80, length 94 bytes (+ int3 padding); `ret` (__cdecl), the result
// on the x87 stack.
// Callee: SecondsToMixBlocks(float) @ 0x00C59B00. Globals: 32768.0 (double) @ 0x012A2428, -1.0f @ 0x0122DEE0,
// 0.0f @ 0x0122DEDC.
// Callers: the envelope update 0x00C59E40.. (decay: `ExponentialDecayStep(1.0f, decay)` at 0x00C59F00/0x00C59F21) and
// the release in 0x00C5A480 (0x00C5A4A2): the first argument is the start level, pushed by every caller but unused
// (the ramp family's shared (from, ..., seconds) argument order; the linear and pitch ramps take a `to` as well).
// Label: the manifest's "Math_ComputeExponent" is a propagated catalog name; descriptive name (GameCODA, no PDB).
// Shape: the same parameter reuse as LinearRampStep; under /O2 `exp` and `log` are true intrinsics (`fldln2 ; fxch ;
// fyl2x` then `fldl2e ; fmulp ; ... f2xm1 ; fscale`), log(32768.0) is not folded.
extern "C" double __cdecl exp(double value);
extern "C" double __cdecl log(double value);
#pragma intrinsic(exp, log)
long __cdecl SecondsToMixBlocks(float seconds);     // 0x00C59B00

float __cdecl ExponentialDecayStep(float from, float seconds)
{
    seconds = (float)SecondsToMixBlocks(seconds);
    if (seconds != 0.0f)
        return (float)exp(log(32768.0) * (-1.0f / seconds));
    return 0.0f;
}