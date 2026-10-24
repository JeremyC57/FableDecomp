// C2DVector::operator/=(float divisor) -- scale both components by the reciprocal of the divisor
// Leaf: ??_0C2DVector
// Retail: Fable.exe @ 0x0066F990, 27 bytes, __thiscall `ret 4`, returns *this in eax. One fdiv into the 1.0f pool
// constant (0x0122DED8), then the reciprocal is duplicated (fld st(0)) and multiplied into X then Y.
// `inv * X` order: VC7.1 loads the named reciprocal first and multiplies the member in from memory.
class C2DVector {
public:
    float X;    // +0x00
    float Y;    // +0x04
    C2DVector& operator/=(float divisor);
};
C2DVector& C2DVector::operator/=(float divisor)
{
    float inv = 1.0f / divisor;
    X *= inv;
    Y *= inv;
    return *this;
}
