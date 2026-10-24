// NShape::IsVectorBetween2OthersInThePlane(const C3DVector& a, const C3DVector& b, const C3DVector& c) -- true when the normalised c is closer in direction to both a and b than a and b are to each other
// Grade: RELOCATION_MATCH (near-miss retry agent901) -- C3DVector gets its memberwise user copy constructor (X, Y, Z), so the third (stack-parameter) copy loads its pointer into eax and stores X first as retail; the implicit POD copy picked ecx
// Leaf: ?IsVectorBetween2OthersInThePlane@NShape@@YI_NABUC3DVector@@00@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0075FF00, 221 bytes (+int3 pad to 224), `ret 4`.
// Identity: Ego_r.pdb ?IsVectorBetween2OthersInThePlane@NShape@@YI_NABVC3DVector@@00@Z (a __fastcall free function:
//   a in ecx, b in edx, c on the stack). Callers: 0x00760842 and four absolute references (function-pointer uses).
// Callees: C3DVector::Normalise() @ 0x00A14440 (three times).
// Shape: three local copies (C3DVector's memberwise copy ctor, through integer registers) normalised in place, then
//   `dot(a,c) > dot(a,b) && dot(b,c) > dot(a,b)` with dot(a,b) kept on the x87 stack across both compares (the
//   `fstp st(0)` on each exit) and C3DVector::Dot's products evaluated z, y, x.
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    C3DVector& Normalise();                       // 0x00A14440
    const float Dot(const C3DVector& v) const { return X * v.X + Y * v.Y + Z * v.Z; }
};

namespace NShape {
bool __fastcall IsVectorBetween2OthersInThePlane(const C3DVector& a, const C3DVector& b, const C3DVector& c);

bool __fastcall IsVectorBetween2OthersInThePlane(const C3DVector& a, const C3DVector& b, const C3DVector& c)
{
    C3DVector na = a;
    na.Normalise();
    C3DVector nb = b;
    nb.Normalise();
    C3DVector nc = c;
    nc.Normalise();
    float ab = na.Dot(nb);
    return na.Dot(nc) > ab && nb.Dot(nc) > ab;
}
}