// GFPackFloat(float value, unsigned long numBits, float minValue, float maxValue, unsigned long numDecimalPlaces) -- quantises a float: rounds it to numDecimalPlaces decimals, normalises it into [minValue, maxValue] and scales it to a numBits-bit integer
// Grade: DIFFER(135v110) -- retail_asm candidate: both float->long conversions are an inline-asm `fld ; fistp dword` helper (round to nearest through a frame word), which VC7.1 cannot emit from C++; this is the C++ reference body.
// Leaf: ?GFPackFloat@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF8D50, length 110 bytes, __fastcall (numBits in ecx -- `shl eax,cl` --,
//   numDecimalPlaces in edx -- the `dec edx ; fmul 10.0` loop --, the three floats on the stack), `ret 0Ch`.
// Signature from FableWin.pdb: ?GFPackFloat@@YIKMKMMK@Z (the paired ?GFUnpackFloat@@YIMKKMMK@Z is its inverse). No
//   direct caller in retail.
// Constants: 1.0f @ 0x0122DED8, 10.0f @ 0x01237F0C.
// Retail-asm evidence: `fstp [esp+0Ch] ; fld [esp+0Ch] ; fistp dword ptr [esp+4]` and `fstp [esp+10h] ; fld [esp+10h] ;
//   fistp dword ptr [esp+0Ch]` -- a float spilled and reloaded straight into a bare fistp, the TLC GFFloatToLong inline
//   asm (C++ conversions call _ftol2 / set the rounding mode).
// Label: catalog "Audio_CalculateFilterParameter" is a guess.
#include <math.h>

inline long GFFloatToLong(float f) { return (long)floor(f + 0.5f); }   // C++ stand-in for the inline-asm fld/fistp

unsigned long __fastcall GFPackFloat(float value, unsigned long numBits, float minValue, float maxValue, unsigned long numDecimalPlaces)
{
    float scale = 1.0f;
    for (unsigned long i = numDecimalPlaces; i != 0; --i)
        scale *= 10.0f;
    float rounded = (float)GFFloatToLong(value * scale) / scale;
    return (unsigned long)GFFloatToLong((rounded - minValue) / (maxValue - minValue) * ((float)(long)(1 << numBits) - 1.0f));
}
