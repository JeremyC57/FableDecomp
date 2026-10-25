// MixDelayTapRampDown(const short* source, const SDelayTap* tap, long* accumulator) -- fade a delay tap out over one 128-sample block: accumulator[i] += (history[i - delay] * gain) >> 15 with the Q15 gain falling from the tap's gain by its step each sample (nothing when the gain is 0)
// Leaf: ?MixDelayTapRampDown@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6CB80, 143 bytes, `ret` (__cdecl).
// Module: libpk's software mixer, the multi-tap delay effect of vtable 0x012BED7C (ctor 0x00C6CA20, landed). Its
//   per-block Process 0x00C6CC60 (slot 2) copies 128 input samples into the 16-byte aligned history buffer (+0x178),
//   then mixes four 8-byte delay taps (sets at +0x1dc current / +0x1e0 previous) into the outputs: after a parameter
//   change (+0x24 flag) the previous taps ramp down and the new ones ramp up, otherwise MixDelayTap 0x00C6CC10 runs
//   (inlined there for the first two outputs). Each tap reads `delay` samples back into the history.
// Layout: local model (no PDB/RTTI; names descriptive). __cdecl free functions (callers pop with `add esp`).
// Callers: 0x00C6CCA4, 0x00C6CCBE, 0x00C6CCEF, 0x00C6CD09 (Process 0x00C6CC60).
// Callees: none.
// Shape: as MixDelayTapRampUp 0x00C6CAF0 (indexed accumulator, pointer history); the step is negated as a short
//   (`short step = -tap->m_step;` = `neg ax ; movsx esi,ax`) and the start gain zero-extends (`movzx ecx,si`).
struct SDelayTap {                        // 8 bytes, four per tap set (the effect's +0x1dc / +0x1e0 sets)
    long           m_delay;               // +0x00 samples back into the history
    unsigned short m_gain;                // +0x04 Q15 gain
    short          m_step;                // +0x06 Q15 gain step per sample (ramps)
};
void MixDelayTapRampDown(const short* source, const SDelayTap* tap, long* accumulator)
{
    unsigned short start = tap->m_gain;
    if (start == 0)
        return;
    short step = -tap->m_step;
    const short* samples = source - tap->m_delay;
    long gain = start;
    for (int i = 0; i < 128; ++i) {
        accumulator[i] += (*samples++ * gain) >> 15;
        gain += step;
    }
}