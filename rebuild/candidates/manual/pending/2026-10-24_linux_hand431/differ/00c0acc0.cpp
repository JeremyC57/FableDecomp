// _global::MultiplyMaskedFactors(const float* factors, unsigned long mask, float value, unsigned long count) -- multiply value by every factor whose bit is set in mask (bit i <-> factors[i]), walking at most count factors and stopping early once the mask is exhausted or the product is zero
// Grade: DIFFER(74v74) -- register allocation only: retail moves the factor pointer to esi and loads the count into ecx (`mov esi,ecx ; mov ecx,[esp+0Ch]`); VC7.1 here keeps the pointer in ecx and the count in esi (the sibling 0x00C0B180 has exactly that allocation in retail)
// Leaf: MultiplyMaskedFactors
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C0ACC0, 74 bytes, `__fastcall` (factors in ecx, mask in edx, value and
//   count on the stack, `ret 8`, result in st0). Descriptive name: the manifest's "Graphics_MultiplyFloatByBitMask"
//   is propagated. The only caller is the parameter block's pitch query 0x00C0B130 (the eight pitch factors at +0x344).
// Constants: 0.0 @ 0x0122DEDC.
// Tried: a named pointer local, an index loop (73 bytes), a separate down-counter (moves the count load before the
//   zero test) -- none moves the pointer out of ecx.
// Shape: the product stays on the x87 stack; the zero test is `fld 0 ; fld st1 ; fucompp` at the top and the
//   bottom of the loop (the rotated `while`), the count counted down in ecx.
static const float kZero = 0.0f;                // 0x0122DEDC

float __fastcall MultiplyMaskedFactors(const float* factors, unsigned long mask, float value, unsigned long count)
{
    while (value != kZero && mask != 0 && count != 0) {
        if (mask & 1)
            value *= *factors;
        --count;
        mask >>= 1;
        ++factors;
    }
    return value;
}
