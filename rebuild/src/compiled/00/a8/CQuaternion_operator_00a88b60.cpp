// CQuaternion::operator*(const CQuaternion&) const -- the Hamilton product this * q (scalar part W)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A88B60, length 168 bytes, `ret 8` (hidden result pointer, then q).
// Leaf: ??DCQuaternion
// Shape: the product is built in a named local, scalar part first, then X, Y, Z, and returned by the implicit copy
// (dword moves through edx, interleaved with the last component's arithmetic); each component is one expression whose
// operand order follows retail's load order. CQuaternion X/Y/Z/W from the PDB (landed operator*= 0x00A88C10 calls this).
class CQuaternion { public:
    float X, Y, Z, W;
    CQuaternion operator*(const CQuaternion& q) const;
};

CQuaternion CQuaternion::operator*(const CQuaternion& q) const
{
    CQuaternion r;
    r.W = W * q.W - q.X * X - Y * q.Y - Z * q.Z;
    r.X = W * q.X + X * q.W + Y * q.Z - Z * q.Y;
    r.Y = q.Y * W + Y * q.W + Z * q.X - q.Z * X;
    r.Z = q.Z * W + q.Y * X + Z * q.W - Y * q.X;
    return r;
}