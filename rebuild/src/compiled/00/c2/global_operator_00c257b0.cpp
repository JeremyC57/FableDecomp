// operator-(const C3DVector& a, const C3DVector& b) -- component-wise difference, returned by value (__fastcall under /Gr: the hidden result in ecx, `a` in edx, `b` on the stack)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C257B0, length 48 bytes.
// Callees: none. Shape: the three differences computed Z, Y, X onto the FPU stack (the constructor's arguments right to left) and stored X, Y, Z into the hidden result, `ret 4`.
// The chunk label "CGuiWindow::OnBeginDrag" is propagated (0x00C2xxxx module).
// Leaf: ??G
class C3DVector { public:
    float x, y, z;
    C3DVector(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
};

C3DVector __fastcall operator-(const C3DVector& a, const C3DVector& b)
{
    return C3DVector(a.x - b.x, a.y - b.y, a.z - b.z);
}