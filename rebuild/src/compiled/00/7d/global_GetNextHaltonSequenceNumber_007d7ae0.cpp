// GetNextHaltonSequenceNumber(float& value, int base) -- advance a Halton (radical-inverse) sequence value to the next element in place without the index: find the first power 1/base^k below 1 - value and apply the digit carry (value += (base + 1) * h - 1)
// Leaf: ?GetNextHaltonSequenceNumber@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x007D7AE0, 94 bytes (+2 int3), __fastcall (value reference in ecx, base in edx),
//   plain `ret`.
// Identity: Ego_r.pdb name set ?GetNextHaltonSequenceNumber@@YIXAAMH@Z (void __fastcall (float&, int)); one caller
//   (0x007D9359). The catalog label is right.
// Shape: the incremental Halton step `double h = 1.0; while (h >= 1 - value) h *= 1.0 / base; value += (base + 1.0)
//   * h - 1.0;` -- the entry test is folded against the 1.0 constant, the loop-invariant `1.0 / base` and `1 - value`
//   hoisted onto the x87 stack (the loop keeps h, 1/base and 1 - value there). Constants 1.0f @ 0x0122DED8,
//   1.0 (double) @ 0x01237F00.
void __fastcall GetNextHaltonSequenceNumber(float& value, int base)
{
    double h = 1.0;
    while (h >= 1 - value)
        h *= 1.0 / base;
    value += (base + 1.0) * h - 1.0;
}