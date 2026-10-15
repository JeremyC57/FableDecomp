// SRegionLookupKey00c46550::SRegionLookupKey00c46550(const C2DVector& position) -- build a 12-byte lookup key: copy the 2D vector and append its length
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C46550, length 31 bytes (`ret 4`). Callees: C2DVector::GetLength @ 0x00C46E90
// (`sqrt(X*X + Y*Y)` on ecx, x87 result; manifest "Math_VectorMagnitude").
// Label correction: the manifest calls this a CRegisteredTCInfo constructor (a propagated name; the PDB CRegisteredTCInfo is
// {AllocFunc, DriverType, CCharString Name} and its real constructor is 0x004D2EF0). The bytes build {X, Y, length}: the
// same three-word key that the landed 0x00C465F0 (unattributed MarkRegion, EH-framed) builds inline on its stack and in its
// 16-byte nodes, so this is the out-of-line copy of that key's constructor; the class is named by address.
// Shape: the vector is copied whole (implicit copy: two integer moves through eax/edx, no x87), then GetLength() runs on the
// argument still in ecx and the result is stored with `fstp [esi+8]`.
// Leaf: ??0SRegionLookupKey00c46550
struct C2DVector {
    float X, Y;
    float GetLength() const;                            // 0x00C46E90
};

struct SRegionLookupKey00c46550 {
    C2DVector Position;                                 // +0x00
    float     Length;                                   // +0x08
    SRegionLookupKey00c46550(const C2DVector& position);
};

SRegionLookupKey00c46550::SRegionLookupKey00c46550(const C2DVector& position)
    : Position(position), Length(position.GetLength())
{
}