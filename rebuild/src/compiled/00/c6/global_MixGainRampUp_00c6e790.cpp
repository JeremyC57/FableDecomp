// MixGainRampUp(const short* source, const SGainRamp* ramp, long* accumulator) -- fade a source in over one 128-sample block: accumulator[i] += (source[i] * gain) >> 15 with the Q15 gain rising from 0 by the record's step each sample (nothing when the step is 0); byte-identical copy of 0x00C6DA40 in the 0x012BEDB8 effect's TU
// Leaf: ?MixGainRampUp@@YAXPBFPBUSGainRamp@@PAJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6E790, 139 bytes, `ret` (__cdecl).
// Module: libpk's software mixer: the per-block Process of the vtable 0x012BED90 effect (0x00C6DBA0, slot 2; ctor
//   0x00C6D9D0 landed) ramps two 4-byte gain records over a 128-sample block through MixGainRampUp 0x00C6DA40 /
//   MixGainRampDown 0x00C6DAD0; the vtable 0x012BEDB8 effect (0x00C6E8F0, slot 2; ctor 0x00C6E760) has byte-identical
//   private copies at 0x00C6E790 / 0x00C6E820 (separate TU).
// Layout: local model (no PDB/RTTI; names descriptive). __cdecl free functions (callers pop with `add esp`).
// Callers: 0x00C6E981, 0x00C6E98F, 0x00C6E9B3, 0x00C6E9C1.
// Callees: none.
// Shape: the 128-step loop unrolled by 4 with both pointers strength-reduced and biased by two elements (`add eax,8`,
//   `add edx,4`, offsets -8..+4 / -4..+2), stepped accumulator-first at the loop end (`add eax,10h ; add edx,8`).
struct SGainRamp {                        // 4 bytes: a Q15 gain and its per-sample step
    unsigned short m_gain;                // +0x00
    short          m_step;                // +0x02
};
void MixGainRampUp(const short* source, const SGainRamp* ramp, long* accumulator)
{
    short step = ramp->m_step;
    if (step == 0)
        return;
    long gain = 0;
    for (int i = 0; i < 128; ++i) {
        // The accumulator is indexed through its own copy of the loop counter: VC7.1 then steps the accumulator pointer
        // before the source pointer both at loop entry and at the loop end (retail 0x00C6E7AE `add eax,8 ; add edx,4`,
        // 0x00C6E80D `add eax,10h ; add edx,8`); indexing both arrays by `i` steps them source-first at the loop end.
        const int sample = i;
        accumulator[sample] += (source[i] * gain) >> 15;
        gain += step;
    }
}