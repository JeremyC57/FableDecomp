// C2DVector::Normalise() -- returns *this; scale the vector to unit length with the table-driven fast 1/sqrt
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A14540, 128 bytes, no calls. Globals: the 128-entry mantissa
// table of the reciprocal square root @ 0x013CE558, 3.0f @ 0x0122DED4, 0.5f @ 0x0122F59C.
// lenSq = X*X + Y*Y goes through the frame to be read as bits; the estimate's exponent is
// (380 - e) / 2 (VC folds `>> 1 << 23` into `shl 22 ; and 0xFF800000`), its mantissa the table entry
// for the top 7 mantissa bits; one Newton step `(3 - r*r*lenSq) * r * 0.5` refines it.
// The leading `mov eax,ecx` is the `return *this` result register holding `this` all the way.
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558

struct C2DVector {
    float X;    // +0x00
    float Y;    // +0x04
    C2DVector& Normalise();
};

C2DVector& C2DVector::Normalise()
{
    float x = X;
    float y = Y;
    float lengthSquared = x * x + y * y;
    unsigned long bits = *(unsigned long*)&lengthSquared;
    unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                             (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
    float r = *(float*)&estimate;
    r = (3.0f - r * r * lengthSquared) * r * 0.5f;
    X *= r;
    Y *= r;
    return *this;
}
