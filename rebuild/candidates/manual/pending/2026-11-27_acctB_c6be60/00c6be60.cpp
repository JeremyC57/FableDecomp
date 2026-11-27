#include <math.h>
// SAL_CalcMacroFxGains -- descriptive, not a PDB name. The landed caller CSalSw3DVoice::CalcParams 0x00C54C10
// already binds ?SAL_CalcMacroFxGains@@YAXMMMMPAM@Z. No symbol in Ego_r or FableWin.
// Retail-asm: 0x00C6BE89 fld dword ptr [esp] ; 0x00C6BE8C fistp dword ptr [esp+4] -- libpk FtoL via libpk/FtoL.h.
#include "fable_compiler.h"
#include "libpk/FtoL.h"
FABLE_FORCEINLINE long FloatToLongTrunc(float value)
{
    long result;
    value = value - 0.5f;
    LIBPK_FTOL(result, value);
    if (result < 0)
        ++result;
    return result;
}
static const float kOctaveScale = 1.0f / 3.0f;
static const float kMinLevel = -90.0f;
static const float kLevelScale = 1.0f / 6.0f;
extern unsigned char g_frequencyTable[][31];
// Landed body of 0x00C6BDC0, kept in this TU so the column stays in edx across the call.
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
extern const float g_directionGainsA[32][80];
extern const float g_directionGainsB[32][80];
void __cdecl SAL_CalcMacroFxGains(float fAzimuth, float fElevation, float fDistance, float fMacroFx, float* pfGains)
{
    if (fMacroFx == 0.0f) {
        pfGains[0] = 1.0f;
        pfGains[1] = 1.0f;
        return;
    }
    unsigned long column = FloatToLongTrunc(fDistance * 100.0f);
    if (column >= 100) {
        pfGains[0] = 1.0f;
        pfGains[1] = 1.0f;
        return;
    }
    if (column < 20)
        column = 20;
    column -= 20;
    unsigned long row = GetFrequencyTableEntry(fAzimuth, fElevation);
    if (fAzimuth < 0.0f) {
        pfGains[0] = g_directionGainsA[row][column];
        pfGains[1] = g_directionGainsB[row][column];
    } else {
        pfGains[0] = g_directionGainsB[row][column];
        pfGains[1] = g_directionGainsA[row][column];
    }
    pfGains[0] = (float)pow(pfGains[0], fMacroFx);
    pfGains[1] = (float)pow(pfGains[1], fMacroFx);
}
