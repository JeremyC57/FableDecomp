// C3DVector::operator==(const C3DVector&) -- exact component-wise equality of two vectors (x, then y, then z; any unordered or unequal pair answers 0)
// Leaf: ??8C3DVector
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C256F0, length 52 bytes. Callers: 0x00C10E8F/0x00C10EA3/
// 0x00C10EB1 (the landed CEnginePrimitiveRenderer2D::SetLightingParameters calls it `__fastcall` on its
// three vector members), 0x00C10F7E, 0x00C20378.
// Label: the catalog name "CTriangleInfo::IsIdentical" is propagated; the body is the C3DVector equality.
// Shape: a `__fastcall` member (this in ecx, the other vector in edx, plain `ret`); each compare loads the
// right operand first (`fld [edx] ; fld [ecx] ; fucompp ; test ah,44h ; jp`); the `&&` chain shares one
// `xor eax,eax` false tail and the true exit is `mov eax,1` (an int result, not bool).
struct C3DVector {
    float x, y, z;
    int __fastcall operator==(const C3DVector& other) const;
};

int __fastcall C3DVector::operator==(const C3DVector& other) const
{
    return x == other.x && y == other.y && z == other.z;
}