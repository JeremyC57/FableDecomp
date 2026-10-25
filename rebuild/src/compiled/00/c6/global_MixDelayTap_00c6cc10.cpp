// MixDelayTap(const short* source, unsigned long count, const SDelayTap* tap, long* accumulator) -- mix `count` samples of a delay tap at its constant Q15 gain: *accumulator++ += (history sample * gain) >> 15 (nothing when the gain is 0)
// Leaf: ?MixDelayTap@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6CC10, 76 bytes (+ int3 padding), `ret` (__cdecl).
// Module: libpk's software mixer, the multi-tap delay effect of vtable 0x012BED7C (ctor 0x00C6CA20, landed). Its
//   per-block Process 0x00C6CC60 (slot 2) copies 128 input samples into the 16-byte aligned history buffer (+0x178),
//   then mixes four 8-byte delay taps (sets at +0x1dc current / +0x1e0 previous) into the outputs: after a parameter
//   change (+0x24 flag) the previous taps ramp down and the new ones ramp up, otherwise MixDelayTap 0x00C6CC10 runs
//   (inlined there for the first two outputs). Each tap reads `delay` samples back into the history.
// Layout: local model (no PDB/RTTI; names descriptive). __cdecl free functions (callers pop with `add esp`).
// Callers: 0x00C6CE9B, 0x00C6CEBA (Process 0x00C6CC60, count 0x80).
// Callees: none.
// Shape: post-incremented pointers (`add ecx,2 ; add eax,4 ; dec edx ; jne`, read-add-store through ebx); the
//   accumulator copied into a named local `out` before the history pointer is formed loads it into eax early, so
//   the gain word takes si (`push esi` first, `mov si,[ecx+4] ; test si,si`), as retail.
struct SDelayTap {                        // 8 bytes, four per tap set (the effect's +0x1dc / +0x1e0 sets)
    long           m_delay;               // +0x00 samples back into the history
    unsigned short m_gain;                // +0x04 Q15 gain
    short          m_step;                // +0x06 Q15 gain step per sample (ramps)
};
void MixDelayTap(const short* source, unsigned long count, const SDelayTap* tap, long* accumulator)
{
    unsigned short gain = tap->m_gain;
    if (gain == 0)
        return;
    long* out = accumulator;
    const short* samples = source - tap->m_delay;
    for (unsigned long i = 0; i < count; ++i)
        *out++ += (*samples++ * gain) >> 15;
}