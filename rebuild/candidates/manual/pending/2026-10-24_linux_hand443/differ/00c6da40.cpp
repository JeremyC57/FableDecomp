// MixGainRampUp(const short* source, const SGainRamp* ramp, long* accumulator) -- fade a source in over one 128-sample block: accumulator[i] += (source[i] * gain) >> 15 with the Q15 gain rising from 0 by the record's step each sample (nothing when the step is 0)
// Grade: DIFFER(139v139) -- scheduling only: the unrolled loop's two pointer steps are swapped (retail `add eax,10h ; add edx,8`, ours `add edx,8 ; add eax,10h`); every other byte matches | tried: index form; post-incremented source (unbiased, 129v139); `acc[i] = acc[i] + ...`; a `scaled` local | next: lane gap: induction-variable order (compiler-build artefact)
// Leaf: ?MixGainRampUp@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6DA40, 139 bytes, `ret` (__cdecl).
// Module: libpk's software mixer: the per-block Process of the vtable 0x012BED90 effect (0x00C6DBA0, slot 2; ctor
//   0x00C6D9D0 landed) ramps two 4-byte gain records over a 128-sample block through MixGainRampUp 0x00C6DA40 /
//   MixGainRampDown 0x00C6DAD0; the vtable 0x012BEDB8 effect (0x00C6E8F0, slot 2; ctor 0x00C6E760) has byte-identical
//   private copies at 0x00C6E790 / 0x00C6E820 (separate TU).
// Layout: local model (no PDB/RTTI; names descriptive). __cdecl free functions (callers pop with `add esp`).
// Callers: 0x00C6DC39, 0x00C6DC49, 0x00C6DC70, 0x00C6DC81.
// Callees: none.
// Shape: the 128-step loop unrolled by 4 with both pointers indexed (each biased by two elements: `add eax,8`,
//   `add edx,4`, offsets -8..+4 / -4..+2), as retail; only the order of the two pointer steps at the loop end differs.
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
        accumulator[i] += (source[i] * gain) >> 15;
        gain += step;
    }
}
