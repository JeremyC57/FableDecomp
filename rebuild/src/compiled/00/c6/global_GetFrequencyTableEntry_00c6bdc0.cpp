// GetFrequencyTableEntry(float octaves, float level) -- libpk audio: look up a byte in the 31-wide response table from a rounded octave magnitude (row) and a level mapped by a scale and offset (column)
// Leaf: ?GetFrequencyTableEntry@@
// Retail-asm: 0x00C6BDEC / 0x00C6BE05 `fld dword ptr [esp+8]` ; 0x00C6BDF0 / 0x00C6BE09 `fistp dword ptr [esp]` ; 0x00C6BE34 `fld dword ptr [esp+8]` ; 0x00C6BE38 `fistp dword ptr [esp+0Ch]` -- libpk's inline-assembly truncation helper (rasm1 family (a)) expanded three times: the value less 0.5 is stored to the dead `octaves` argument slot (the helper's by-value parameter) and reloaded for a bare dword fistp into a frame word (the first result in the local, the second in the dead `level` slot), then the C++ `< 0 ? +1` fix-up (`test ; jge ; inc`); VC7.1 C++ `(long)x` calls __ftol2, /QIfist emits `fistp qword` and /arch:SSE2 cvttss2si (rasm1 probe family (a)). The sign test, the arithmetic and the table read are C++.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C6BDC0, 146 bytes (+14 int3), __cdecl, plain `ret`. Constants (all pool literals): 0.0f @ 0x0122DEDC,
//   0.5f @ 0x0122F59C, 1/3 @ 0x0125A4DC, -90.0f @ 0x012B7B60, 1/6 @ 0x0125D640; the byte table @
//   0x012B9610 (31 columns). Called from 0x00C6BECB. The catalog name "Audio_GetFrequencyTableEntry" is descriptive.
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
static const float kOctaveScale = 1.0f / 3.0f;              // 0x3EAAAAAB @ 0x0125A4DC (a pool constant: the fmul operand)
static const float kMinLevel = -90.0f;                      // 0xC2B40000 @ 0x012B7B60
static const float kLevelScale = 1.0f / 6.0f;               // 0x3E2AAAAB @ 0x0125D640
extern unsigned char g_frequencyTable[][31];                // 0x012B9610
unsigned long __cdecl GetFrequencyTableEntry(float octaves, float level)
{
    long row;
    if (octaves < 0.0f)
        row = FloatToLongTrunc(0.5f - octaves * kOctaveScale);
    else
        row = FloatToLongTrunc(octaves * kOctaveScale + 0.5f);
    long column = FloatToLongTrunc((level - kMinLevel) * kLevelScale + 0.5f);
    return g_frequencyTable[row][column];
}