// C2DVector::GetLength() const -- Euclidean length of a 2D float vector, sqrt(X*X + Y*Y), returned on the x87 stack
// Leaf: ?GetLength@C2DVector@@QBEMXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C46E90, 22 bytes (+ int3 padding), __fastcall, plain `ret`.
// Callers: the landed region-lookup key constructors 0x00C46550 / 0x00C46570 and MarkRegion 0x00C465F0 (twice), which
//   name it C2DVector::GetLength; the manifest's "Math_VectorMagnitude" describes it.
// Shape: the plain expression -- VC7.1 loads Y then X once, squares from the register copies (`fld st0 ; fmul st1`,
//   `fld st2 ; fmul st3`), adds and runs the intrinsic `fsqrt`, then pops the two copies (`fstp st2 ; fstp st0`);
//   no rounding store for the float result without /Op.
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)

struct C2DVector {
    float X;                                    // +0x00
    float Y;                                    // +0x04
    float GetLength() const;
};

float C2DVector::GetLength() const
{
    return (float)sqrt(X * X + Y * Y);
}