// libvorbis::vorbis_dBquant(const float* x) -- libvorbis floor1.c: quantise a dB value to the 10-bit floor-1 scale (x*7.3142857+1023.5, clamped to 0..1023)
// Leaf: _vorbis_dBquant
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA1E50, 43 bytes. libvorbis lib/floor1.c `static int vorbis_dBquant(const float *x)`;
//   x arrives in eax (VC7.1 private convention), `_ftol2` 0x00BFEA70 converts; constants 7.3142857f @ 0x012BF35C,
//   1023.5f @ 0x012BF358. Callers 0x00CA2124 / 0x00CA24CA / 0x00CA2596 (floor1.c fit_line / floor1_fit).
// Shape (retry1): parked as an "LTCG custom register convention" gap; it is a same-TU static: inside
//   `extern "C" { static __declspec(noinline) }` with a same-TU caller VC7.1 passes x in eax as retail does.
// Label: manifest "Clamp_To10BitValue" is a descriptive placeholder; the real name is the floor1.c static.
extern "C" {
static __declspec(noinline) int vorbis_dBquant(const float* x)
{
    int i = *x * 7.3142857f + 1023.5f;
    if (i > 1023) return (1023);
    if (i < 0) return (0);
    return i;
}

// Same-TU caller (the vorbis_dBquant calls of floor1.c, 0x00CA2124 / 0x00CA24CA / 0x00CA2596), present so the static
// gets retail's private convention. Not graded.
int __cdecl vorbis_floor1_dBquant_caller(const float* flr, int n, int* out)
{
    int i;
    for (i = 0; i < n; i++) out[i] = vorbis_dBquant(flr + i);
    return n;
}
}