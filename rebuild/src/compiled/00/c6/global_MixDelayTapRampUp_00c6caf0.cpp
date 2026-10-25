// MixDelayTapRampUp(const short* source, const SDelayTap* tap, long* accumulator) -- fade a delay tap in over one 128-sample block: accumulator[i] += (history[i - delay] * gain) >> 15 with the Q15 gain rising from 0 by the tap's step each sample (nothing when the step is 0)
// Leaf: ?MixDelayTapRampUp@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6CAF0, 139 bytes (+ int3 padding), `ret` (__cdecl).
// Module: libpk's software mixer, the multi-tap delay effect of vtable 0x012BED7C (ctor 0x00C6CA20, landed). Its
//   per-block Process 0x00C6CC60 (slot 2) copies 128 input samples into the 16-byte aligned history buffer (+0x178),
//   then mixes four 8-byte delay taps (sets at +0x1dc current / +0x1e0 previous) into the outputs: after a parameter
//   change (+0x24 flag) the previous taps ramp down and the new ones ramp up, otherwise MixDelayTap 0x00C6CC10 runs
//   (inlined there for the first two outputs). Each tap reads `delay` samples back into the history.
// Layout: local model (no PDB/RTTI; names descriptive). __cdecl free functions (callers pop with `add esp`).
// Callers: 0x00C6CD36, 0x00C6CD50, 0x00C6CD81, 0x00C6CD9B (Process 0x00C6CC60).
// Callees: none.
// Shape: VC7.1 unrolls the 128-step loop by 4 (`mov edi,20h ; dec edi ; jne`); the accumulator is indexed (VC biases
//   its pointer by two elements: `add eax,8`, offsets -8..+4) while the history is read through a post-incremented
//   pointer local (offsets 0..6, no bias) -- the all-index form biases both, the all-pointer form unrolls by 2.
struct SDelayTap {                        // 8 bytes, four per tap set (the effect's +0x1dc / +0x1e0 sets)
    long           m_delay;               // +0x00 samples back into the history
    unsigned short m_gain;                // +0x04 Q15 gain
    short          m_step;                // +0x06 Q15 gain step per sample (ramps)
};
void MixDelayTapRampUp(const short* source, const SDelayTap* tap, long* accumulator)
{
    short step = tap->m_step;
    if (step == 0)
        return;
    const short* samples = source - tap->m_delay;
    long gain = 0;
    for (int i = 0; i < 128; ++i) {
        accumulator[i] += (*samples++ * gain) >> 15;
        gain += step;
    }
}