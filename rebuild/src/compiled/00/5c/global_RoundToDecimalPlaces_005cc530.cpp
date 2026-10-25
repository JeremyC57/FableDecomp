// RoundToDecimalPlaces(long decimalPlaces, float value) -- round value to the given number of decimal places: scale = 10^places (1.0f times 10.0f per place), value * scale rounded to the nearest integer, divided back by scale
// Grade: RELOCATION_MATCH (1 grade) -- rasm1 retail_asm: C++ body + a minimal 2-instruction __asm (no control flow) + the Retail-asm evidence line
// Retail-asm: 0x005CC552 `fstp [esp+0Ch]` / 0x005CC556 `fld dword ptr [esp+0Ch]` + 0x005CC55A `fistp dword ptr [esp+4]` -- the engine's inline-assembly float-to-long helper (the product stored to its by-value copy in the dead `value` slot and reloaded, a bare dword fistp into a frame word; the asm barrier also spills `scale` to [esp]). VC7.1 C++ `(long)x` calls __ftol2 and /QIfist (RTM 3077 and QFE 4035, with /Op, /Og or /G5-/G7) emits `fistp qword` into an 8-byte slot, /arch:SSE2 a cvttss2si: no C/C++ form gives the bare dword fistp, so it is the original's `__asm`.
// Leaf: RoundToDecimalPlaces
// Retail: Fable.exe @ 0x005CC530, 59 bytes, `ret 4`: __fastcall-shaped (places in ecx, the float on the stack), result in
// st(0). No direct callers (xref finds none) and no Ego_r.pdb symbol: name from the catalog's descriptive label.
// Constants: 0x0122DED8 = 1.0f (the scale's start), 0x01237F0C = 10.0f (one decimal place).
// Body: float scale = 1.0f; for (long n = places; n; --n) scale *= 10.0f; return (float)Round(value * scale) / scale;
//   the scale lives in st(0) through the loop with `fst [esp]` copies on both paths; the division reads the frame copy.
// the engine's inline-assembly float-to-long helper (round to nearest, the FPU default); expanded in place as a __forceinline
__forceinline long FloatToLong(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}

float __fastcall RoundToDecimalPlaces(long decimalPlaces, float value)
{
    float scale = 1.0f;
    for (long n = decimalPlaces; n != 0; --n)
        scale *= 10.0f;
    long rounded = FloatToLong(value * scale);
    return (float)rounded / scale;
}