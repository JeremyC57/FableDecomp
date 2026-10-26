// C3DVector::operator!=(const C3DVector&) -- inequality of two vectors: 1 as soon as one component pair (x, y, z in order) is not equal, else 0
// Leaf: ??9C3DVector
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C25730, length 52 bytes. Caller: 0x00C3722A.
// Label: the catalog name "CTriangleInfo::IsIdentical" is propagated; the twin of C3DVector::operator==
// 0x00C256F0 with the exits swapped.
// Shape: a `__fastcall` member (this in ecx, the other vector in edx, plain `ret`); right operand loaded
// first in each compare, `||` chain with the shared true tail `mov eax,1` after the `xor eax,eax` exit.
class C3DVector { public:
    float x, y, z;
    int __fastcall operator!=(const C3DVector& other) const;
};

int __fastcall C3DVector::operator!=(const C3DVector& other) const
{
    return x != other.x || y != other.y || z != other.z;
}