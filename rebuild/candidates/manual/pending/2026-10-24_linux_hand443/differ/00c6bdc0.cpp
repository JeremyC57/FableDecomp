// LookupDirectionIndex(float azimuth, float elevation) -- index of the directional filter for a source direction: the byte table 0x012B9610 is 61 rows (|azimuth| in 3-degree steps, rounded) by 31 columns (elevation -90..90 in 6-degree steps, rounded)
// Grade: DIFFER(165v146) -- retail_asm candidate: three bare `fistp dword` truncation-helper expansions (0x00C6BDF0 / 0x00C6BE09 / 0x00C6BE38); ours rounds in C++ + __ftol2 | 1 grade | next: land as retail_asm
// Leaf: ?LookupDirectionIndex@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6BDC0, 146 bytes (+ int3 padding), `ret` (__cdecl).
// Module: libpk's software mixer (descriptive name; the manifest's "Audio_GetFrequencyTableEntry" is propagated).
//   Caller: ComputeDirectionalGains 0x00C6BE60 (which indexes 32-row float tables with the result).
// Retail-asm: 0x00C6BDDC / 0x00C6BDF5 / 0x00C6BE24 `fsub [0.5f] ; fstp [esp+8] ; fld [esp+8] ; fistp dword [esp] ; test ; jge ;
//   inc` -- libpk's inline-assembly truncation helper (three expansions; the first two arms share the tail).
// Shape notes for landing: the sign test is on the azimuth argument itself (`fcomp [0.0f]` before the `* (1/3)`), the
//   negative arm rounds `0.5f - scaled` (fsubr), the elevation is `(elevation - -90.0f) * (1/6) + 0.5f`.
// libpk's inline-assembly float-to-long helper. Retail homes the value in a dead stack slot and rounds it with a bare
// `fld [slot] ; fistp dword [tmp]` (round to nearest, the FPU default). VC7.1 C++ cannot emit that (`(long)x` calls
// __ftol2 and /QIfist emits `fistp qword`), so the body below is a C++ stand-in; at landing it becomes
// the minimal inline-assembly `fld value ; fistp result` body (grade retail_asm, like the landed ROUND of 0x00BE6790).
__forceinline long FloatToLong(float value)
{
    return (long)(value < 0.0f ? value - 0.5f : value + 0.5f);     // stand-in: round to nearest (fistp: ties to even)
}
// truncation toward zero on top of the rounding helper: round(x - 0.5) is floor(x), negatives step back up by one
// (retail: `fsub [0.5f] ; fstp [slot] ; fld [slot] ; fistp dword [tmp] ; test ; jge ; inc`)
__forceinline long FloatToLongTruncate(float value)
{
    long result = FloatToLong(value - 0.5f);
    if (result < 0)
        ++result;
    return result;
}
extern const unsigned char g_directionTable[61][31];     // 0x012B9610
unsigned long __cdecl LookupDirectionIndex(float azimuth, float elevation)
{
    float scaled = azimuth * 0.33333334f;
    long row;
    if (azimuth < 0.0f)
        row = FloatToLongTruncate(0.5f - scaled);
    else
        row = FloatToLongTruncate(scaled + 0.5f);
    long column = FloatToLongTruncate((elevation - -90.0f) * 0.16666667f + 0.5f);
    return g_directionTable[row][column];
}
