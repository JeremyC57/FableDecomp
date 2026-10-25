// MixGainRampDown(const short* source, const SGainRamp* ramp, long* accumulator) -- fade a source out over one 128-sample block (byte-identical copy of 0x00C6DAD0 in the 0x012BEDB8 effect's TU)
// Grade: DIFFER(139v139) -- scheduling only: the unrolled loop's two pointer steps are swapped (retail `add eax,10h ; add edx,8`, ours `add edx,8 ; add eax,10h`); every other byte matches | tried: `gain * source[i]`; `out`/`in` local copies | next: lane gap: induction-variable order (compiler-build artefact)
// Leaf: ?MixGainRampDown@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6E820, 139 bytes, `ret` (__cdecl).
// Module: libpk's software mixer: the per-block Process of the vtable 0x012BED90 effect (0x00C6DBA0, slot 2; ctor
//   0x00C6D9D0 landed) ramps two 4-byte gain records over a 128-sample block through MixGainRampUp 0x00C6DA40 /
//   MixGainRampDown 0x00C6DAD0; the vtable 0x012BEDB8 effect (0x00C6E8F0, slot 2; ctor 0x00C6E760) has byte-identical
//   private copies at 0x00C6E790 / 0x00C6E820 (separate TU).
// Layout: local model (no PDB/RTTI; names descriptive). __cdecl free functions (callers pop with `add esp`).
// Callers: 0x00C6E924, 0x00C6E932, 0x00C6E958, 0x00C6E966.
// Callees: none.
// Shape: the 128-step loop unrolled by 4 with both pointers indexed (each biased by two elements: `add eax,8`,
//   `add edx,4`, offsets -8..+4 / -4..+2), as retail; only the order of the two pointer steps at the loop end differs.
struct SGainRamp {                        // 4 bytes: a Q15 gain and its per-sample step
    unsigned short m_gain;                // +0x00
    short          m_step;                // +0x02
};
void MixGainRampDown(const short* source, const SGainRamp* ramp, long* accumulator)
{
    unsigned short start = ramp->m_gain;
    if (start == 0)
        return;
    short step = -ramp->m_step;
    long gain = start;
    for (int i = 0; i < 128; ++i) {
        accumulator[i] += (source[i] * gain) >> 15;
        gain += step;
    }
}
