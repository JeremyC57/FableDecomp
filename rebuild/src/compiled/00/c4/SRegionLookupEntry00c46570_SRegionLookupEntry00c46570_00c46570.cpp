// SRegionLookupEntry00c46570::SRegionLookupEntry00c46570(const C2DVector&) -- build a 16-byte region lookup entry: a zero leading word, the 2D position and the position's length
// Leaf: ??0SRegionLookupEntry00c46570
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C46570, length 38 bytes (`ret 4`, returns this).
// Callee: C2DVector::GetLength @ 0x00C46E90 (`sqrt(X*X + Y*Y)` on ecx, x87 result; manifest "Math_VectorMagnitude").
// Label correction: the manifest "CTCRotate::SetRotationInfo" is propagated (CTCRotate::SetRotationInfo is 0x0072A530).
// The body is the twin of the landed 0x00C46550 key constructor (SRegionLookupKey00c46550: {X, Y, length}) with one
// leading word cleared first -- the 16-byte node record of the same region lookup (landed 0x00C465F0 builds 16-byte nodes
// inline); no direct callers, so the class is named by address.
// Shape: the leading word is an initialiser stored first (`mov dword ptr [esi],0`), the vector copied whole with two
// integer moves, GetLength() on the argument still in ecx, `fstp [esi+0Ch]`.
class C2DVector { public:
    float X, Y;
    float GetLength() const;                 // 0x00C46E90
};

struct SRegionLookupEntry00c46570 {
    long      Region;                        // +0x00
    C2DVector Position;                      // +0x04
    float     Length;                        // +0x0c
    SRegionLookupEntry00c46570(const C2DVector& position);
};

SRegionLookupEntry00c46570::SRegionLookupEntry00c46570(const C2DVector& position)
    : Region(0), Position(position), Length(position.GetLength())
{
}