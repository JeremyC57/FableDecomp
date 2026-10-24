// GetHaltonSequenceNumber(int index, int base) -- the index-th element of the radical-inverse (Halton/van der Corput) sequence in the given base: the base-b digits of index mirrored about the radix point
// Leaf: ?GetHaltonSequenceNumber@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x007D7A90, 66 bytes (+14 int3), __fastcall (index in ecx, base in edx), float in
//   st(0), plain `ret`.
// Identity: Ego_r.pdb name set ?GetHaltonSequenceNumber@@YIMHH@Z (float __fastcall (int, int)); no direct caller in
//   retail. The catalog label is right.
// Shape: `result = 0; invBase = 1.0f / base (a float frame word, `fst`); frac = invBase; while (index > 0) { digit =
//   index % base; index /= base; result += digit * frac; frac *= invBase; }` -- one idiv for both, the running fraction
//   and the sum kept on the x87 stack. Constants 0.0f @ 0x0122DEDC, 1.0f @ 0x0122DED8.
float __fastcall GetHaltonSequenceNumber(int index, int base)
{
    float result = 0.0f;
    float invBase = 1.0f / base;
    float frac = invBase;
    while (index > 0) {
        int digit = index % base;
        index /= base;
        result += digit * frac;
        frac *= invBase;
    }
    return result;
}