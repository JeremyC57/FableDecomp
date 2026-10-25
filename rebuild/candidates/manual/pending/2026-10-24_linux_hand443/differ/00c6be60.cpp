// ComputeDirectionalGains(float azimuth, float elevation, float frequency, float exponent, SStereoGains* gains) -- the two directional filter gains of a source: unity when the exponent is 0 or trunc(frequency * 100) is 100 or more; else the direction index (LookupDirectionIndex) and column max(that, 20) - 20 select an entry of the two 32x80 float tables (swapped for a negative azimuth), each raised to the exponent
// Grade: DIFFER(250v226) -- retail_asm candidate: bare `fistp dword` truncation helper (0x00C6BE8C) plus an LTCG register allocation (edx live across the call to 0x00C6BDC0, the callee's full eax used); ours rounds in C++ + __ftol2 | 1 grade | next: land as retail_asm, then check the edx-across-call shape (LTCG lane gap)
// Leaf: ?ComputeDirectionalGains@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6BE60, 226 bytes, `ret` (__cdecl).
// Module: libpk's software mixer (descriptive names; the manifest's "Audio_ComputeFrequencyResponse" is propagated).
//   Caller: 0x00C54DAA. Callees: LookupDirectionIndex 0x00C6BDC0, _CIpow 0x00BFEB40 (the /Oi `pow`). Tables:
//   0x012B9D78 / 0x012BC578 (32 rows of 80 floats each).
// Retail-asm: 0x00C6BE80 `fsub [0.5f] ; fstp [esp] ; fld [esp] ; fistp dword [esp+4] ; test ; jge ; inc` -- libpk's
//   inline-assembly truncation helper.
// LTCG: the column (edx, already reduced by 20) stays live across the call to 0x00C6BDC0 and the caller uses the callee's
//   full eax: a link-time-code-generation register allocation VC7.1 /O2 does not reproduce (lane gap on top of the asm).
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
#include <math.h>
struct SStereoGains {
    float First;                                   // +0x00
    float Second;                                  // +0x04
};
extern const float g_directionGainsA[32][80];      // 0x012B9D78
extern const float g_directionGainsB[32][80];      // 0x012BC578
unsigned long __cdecl LookupDirectionIndex(float azimuth, float elevation);   // 0x00C6BDC0
void __cdecl ComputeDirectionalGains(float azimuth, float elevation, float frequency, float exponent, SStereoGains* gains)
{
    if (exponent != 0.0f) {
        unsigned long column = FloatToLongTruncate(frequency * 100.0f);
        if (column < 100) {
            if (column < 20)
                column = 20;
            unsigned long row = LookupDirectionIndex(azimuth, elevation);
            column -= 20;
            if (azimuth < 0.0f) {
                gains->First = g_directionGainsA[row][column];
                gains->Second = g_directionGainsB[row][column];
            } else {
                gains->First = g_directionGainsB[row][column];
                gains->Second = g_directionGainsA[row][column];
            }
            gains->First = (float)pow(gains->First, exponent);
            gains->Second = (float)pow(gains->Second, exponent);
            return;
        }
    }
    gains->First = 1.0f;
    gains->Second = 1.0f;
}
