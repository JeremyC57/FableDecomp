// operator+(const C3DVector&, const C3DVector&) -- the component-wise sum of two 3D vectors, returned by value
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C257E0, length 33 bytes. No callees.
// Leaf: ??H@YI
// The manifest label "CQuadric::operator+=" is a BSim port (0.78) and is rejected (the real CQuadric is eleven
// doubles; the landed caller 0x00C62460 uses this body as C3DVector addition). Shape: `__fastcall` with the hidden
// result pointer in ecx (moved to eax), the left operand in edx, the right operand on the stack (`ret 4`).
// C3DVector has a user constructor, so it returns through the hidden pointer; the constructor's arguments are
// evaluated right to left (Z sum, Y sum, X sum) and stored X, Y, Z as the three `fstp` pop the stack.
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};

C3DVector __fastcall operator+(const C3DVector& a, const C3DVector& b)
{
    return C3DVector(a.X + b.X, a.Y + b.Y, a.Z + b.Z);
}