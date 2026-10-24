// RoundToDecimalPlaces(long decimalPlaces, float value) -- round value to the given number of decimal places: scale = 10^places (1.0f times 10.0f per place), value * scale rounded to the nearest integer, divided back by scale
// Grade: DIFFER(47v59) -- retail rounds with a bare `fld [esp+0Ch] ; fistp dword ptr [esp+4]` (the inline-asm float->int helper: the product spilled into the dead `value` slot, the int into a frame word) and reloads it with `fild`; C++ gives a `__ftol2` call
// Leaf: RoundToDecimalPlaces
// Retail: Fable.exe @ 0x005CC530, 59 bytes, `ret 4`: __fastcall-shaped (places in ecx, the float on the stack), result in
// st(0). No direct callers (xref finds none) and no Ego_r.pdb symbol: name from the catalog's descriptive label.
// Constants: 0x0122DED8 = 1.0f (the scale's start), 0x01237F0C = 10.0f (one decimal place).
// Body: float scale = 1.0f; for (long n = places; n; --n) scale *= 10.0f; return (float)Round(value * scale) / scale;
//   the scale lives in st(0) through the loop with `fst [esp]` copies on both paths; the division reads the frame copy.
// Lane gap: the rounding is the retail inline-asm idiom (CLAUDE.md's `retail_asm` exception names bare `fistp dword`);
// this lane forbids `__asm`, so the row is parked with the pure C++ form (same idiom as 0x005BC3ED in this chunk).
float __fastcall RoundToDecimalPlaces(long decimalPlaces, float value)
{
    float scale = 1.0f;
    for (long n = decimalPlaces; n != 0; --n)
        scale *= 10.0f;
    float scaled = value * scale;
    long rounded = (long)scaled;
    return (float)rounded / scale;
}
