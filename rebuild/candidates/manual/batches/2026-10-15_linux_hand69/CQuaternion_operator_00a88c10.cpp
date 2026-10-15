// CQuaternion::operator*=(const CQuaternion&) -- multiply this quaternion by another (this * q through operator*) and store the product in place, returning *this
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A88C10, length 56 bytes.
// Leaf: ??XCQuaternion
// Callees: CQuaternion::operator*(const CQuaternion&) const @ 0x00A88B60 (`__thiscall`, the 16-byte result through
// the hidden pointer: the frame temporary at [esp+4]).
// Shape: a 16-byte frame for the product; the copy back is the implicit assignment (four dword moves through ecx,
// the destination re-based in edx), then `mov eax,esi` for the reference return. CQuaternion X/Y/Z/W from the PDB.
struct CQuaternion {
    float X, Y, Z, W;
    CQuaternion operator*(const CQuaternion& q) const;   // 0x00A88B60
    CQuaternion& operator*=(const CQuaternion& q);
};

CQuaternion& CQuaternion::operator*=(const CQuaternion& q)
{
    *this = *this * q;
    return *this;
}
