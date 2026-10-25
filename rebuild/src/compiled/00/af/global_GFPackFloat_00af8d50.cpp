// GFPackFloat(float value, unsigned long numBits, float minValue, float maxValue, unsigned long numDecimalPlaces) -- quantises a float: rounds it to numDecimalPlaces decimals, normalises it into [minValue, maxValue] and scales it to a numBits-bit integer
// Grade: RELOCATION_MATCH (1 grade) -- rasm1 retail_asm: C++ body + a minimal 2-instruction __asm (no control flow) + the Retail-asm evidence line
// Retail-asm: 0x00AF8D70 `fstp [esp+0Ch]` / 0x00AF8D74 `fld dword ptr [esp+0Ch]` + 0x00AF8D78 `fistp dword ptr [esp+4]` and 0x00AF8DA8 / 0x00AF8DAC + 0x00AF8DB0 `fistp dword ptr [esp+0Ch]` -- two expansions of the engine's inline-assembly GFFloatToLong helper (by-value copy stored and reloaded, bare dword fistp). VC7.1 C++ `(long)x` calls __ftol2 and /QIfist (RTM 3077 and QFE 4035, with /Op, /Og or /G5-/G7) emits `fistp qword` into an 8-byte slot, /arch:SSE2 a cvttss2si: no C/C++ form gives the bare dword fistp, so it is the original's `__asm`.
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
// the engine's inline-assembly float-to-long helper (round to nearest, the FPU default); expanded in place as a __forceinline
__forceinline long GFFloatToLong(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}

unsigned long __fastcall GFPackFloat(float value, unsigned long numBits, float minValue, float maxValue, unsigned long numDecimalPlaces)
{
    float scale = 1.0f;
    for (unsigned long i = numDecimalPlaces; i != 0; --i)
        scale *= 10.0f;
    float rounded = (float)GFFloatToLong(value * scale) / scale;
    return (unsigned long)GFFloatToLong((rounded - minValue) / (maxValue - minValue) * ((float)(long)(1 << numBits) - 1.0f));
}