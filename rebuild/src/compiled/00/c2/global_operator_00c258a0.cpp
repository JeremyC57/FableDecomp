// operator*(const C3DVector& v, float s) -- a 3D vector scaled by a float, returned by value
// Leaf: ??D@YI
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C258A0, 33 bytes (`ret 4`). The manifest's "GetNewCameraPosFromOcclusionParams"
//   is propagated: the body is the vector-times-scalar operator of the 0x00C25xxx C3DVector helpers (next to the
//   landed operator- 0x00C257B0, operator+ 0x00C257E0, CrossProduct 0x00C25860); eight callers in 0x00C46xxx.
// Shape: `__fastcall` free operator (as the landed operator+/-): the hidden result pointer in ecx (moved to eax), the
//   vector in edx, the scalar on the stack. C3DVector has a user constructor, so it returns through the hidden pointer;
//   the constructor's arguments are evaluated right to left (Z, Y, X products, each `fld s ; fmul [v.c]`, i.e.
//   `v.c * s` with the right operand loaded first) and stored X, Y, Z as the three `fstp` pop the stack.
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};

C3DVector __fastcall operator*(const C3DVector& v, float s)
{
    return C3DVector(v.X * s, v.Y * s, v.Z * s);
}