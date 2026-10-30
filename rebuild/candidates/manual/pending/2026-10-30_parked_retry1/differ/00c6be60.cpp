// ComputeDirectionalGains(float azimuth, float elevation, float frequency, float exponent, SStereoGains* gains) -- the two directional filter gains of a source: unity when the exponent is 0 or trunc(frequency * 100) is 100 or more; else the direction index (GetFrequencyTableEntry) and column max(that, 20) - 20 select an entry of the two 32x80 float tables (swapped for a negative azimuth), each raised to the exponent
// Grade: DIFFER(226v226) (retry1; was 250v226) -- one residue: `sub edx,14h` (column -= 20) sits after the GetFrequencyTableEntry call here, retail schedules it between the argument pushes and the call | grades: callee body in TU (234v226), merged early-exit condition (234v226), two early unity returns (226v226) | next: write `column -= 20;` above the call (3-grade budget spent)
// Shape (retry1): parked as "retail_asm candidate + LTCG register allocation (edx live across the call)"; the truncation is
//   libpk's inline-assembly helper (as in the landed GetFrequencyTableEntry 0x00C6BDC0), and with that callee's body in the
//   TU VC7.1 /O2 knows it leaves edx alone and keeps the column there across the call itself (chunk 585 lever).
// Leaf: ?ComputeDirectionalGains@@YAXMMMMPAUSStereoGains@@@Z
// Retail-asm: 0x00C6BE89 `fld dword ptr [esp]` ; 0x00C6BE8C `fistp dword ptr [esp+4]` -- libpk's inline-assembly truncation helper: frequency * 100 less 0.5 stored to a frame word (the helper's by-value parameter) and reloaded for a bare dword fistp, then the C++ `< 0 ? +1` fix-up (`test ; jge ; inc`); VC7.1 C++ `(long)x` calls __ftol2, /QIfist emits `fistp qword`. The callee GetFrequencyTableEntry 0x00C6BDC0 (landed retail_asm, defined below for its register use) carries the same helper: 0x00C6BDEC / 0x00C6BE05 `fld dword ptr [esp+8]` ; 0x00C6BDF0 / 0x00C6BE09 `fistp dword ptr [esp]` ; 0x00C6BE34 `fld dword ptr [esp+8]` ; 0x00C6BE38 `fistp dword ptr [esp+0Ch]`.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6BE60, 226 bytes, `ret` (__cdecl).
// Module: libpk's software mixer (descriptive names; the manifest's "Audio_ComputeFrequencyResponse" is propagated).
//   Caller: 0x00C54DAA. Callees: GetFrequencyTableEntry 0x00C6BDC0 (landed), _CIpow 0x00BFEB40 (the /Oi `pow`). Tables:
//   0x012B9D78 / 0x012BC578 (32 rows of 80 floats each).
#include <math.h>
// libpk's inline-assembly truncation helper: round-to-nearest of (x - 0.5) by a bare fistp, corrected to truncation for negatives
__forceinline long FloatToLongTrunc(float value)
{
    long result;
    value = value - 0.5f;
    __asm fld value
    __asm fistp result
    if (result < 0)
        ++result;
    return result;
}
static const float kOctaveScale = 1.0f / 3.0f;              // 0x3EAAAAAB @ 0x0125A4DC
static const float kMinLevel = -90.0f;                      // 0xC2B40000 @ 0x012B7B60
static const float kLevelScale = 1.0f / 6.0f;               // 0x3E2AAAAB @ 0x0125D640
extern unsigned char g_frequencyTable[][31];                // 0x012B9610
// 0x00C6BDC0, the landed GetFrequencyTableEntry body (not graded here).
__declspec(noinline) unsigned long __cdecl GetFrequencyTableEntry(float octaves, float level)
{
    long row;
    if (octaves < 0.0f)
        row = FloatToLongTrunc(0.5f - octaves * kOctaveScale);
    else
        row = FloatToLongTrunc(octaves * kOctaveScale + 0.5f);
    long column = FloatToLongTrunc((level - kMinLevel) * kLevelScale + 0.5f);
    return g_frequencyTable[row][column];
}

struct SStereoGains {
    float First;                                   // +0x00
    float Second;                                  // +0x04
};
extern const float g_directionGainsA[32][80];      // 0x012B9D78
extern const float g_directionGainsB[32][80];      // 0x012BC578
void __cdecl ComputeDirectionalGains(float azimuth, float elevation, float frequency, float exponent, SStereoGains* gains)
{
    if (exponent == 0.0f) {
        gains->First = 1.0f;
        gains->Second = 1.0f;
        return;
    }
    unsigned long column = FloatToLongTrunc(frequency * 100.0f);
    if (column >= 100) {
        gains->First = 1.0f;
        gains->Second = 1.0f;
        return;
    }
    if (column < 20)
        column = 20;
    unsigned long row = GetFrequencyTableEntry(azimuth, elevation);
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
}
