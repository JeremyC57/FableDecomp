// CTools::IsVectorBetweenTwoOthersInThePlane(const C3DVector& a, const C3DVector& b, const C3DVector& v) -- with all three directions normalised: true when v is at least as close in angle to a as b is, i.e. a.b >= v.b and a.v >= v.b
// Leaf: IsVectorBetweenTwoOthersInThePlane
// Grade: DIFFER(220v220) -- frame-slot assignment only: retail homes the normalised copies a/b/v at +0x18/+0/+0xc, ours at +0xc/+0/+0x18 (every instruction otherwise identical; the x-first source sums give retail's z-first FPU order, the memberwise copy ctor its ecx/edx/eax copies)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00875040, length 220 bytes (`ret 0Ch`, `this` unused; 224 with padding).
// Callees: C3DVector::Normalise @ 0x00A14440 (x3, on frame copies).
// Shape: three POD copies (three registers each) normalised in argument order; the v.b dot product is a named float
// kept on the FPU across both compares (`test ah,1 ; jne` = `>=`); each dot product sums z, y, x with the right operand
// loaded first (`fld [v.z] ; fmul [b.z]` = `b.z * v.z`).
struct C3DVector {
    float x, y, z;
    C3DVector(const C3DVector& o) : x(o.x), y(o.y), z(o.z) { }
    void Normalise();                                       // 0x00A14440
};

struct CTools {
    bool IsVectorBetweenTwoOthersInThePlane(const C3DVector& a, const C3DVector& b, const C3DVector& v);
};

bool CTools::IsVectorBetweenTwoOthersInThePlane(const C3DVector& a, const C3DVector& b, const C3DVector& v)
{
    C3DVector na = a;
    na.Normalise();
    C3DVector nb = b;
    nb.Normalise();
    C3DVector nv = v;
    nv.Normalise();
    float vb = nb.x * nv.x + nb.y * nv.y + nb.z * nv.z;
    if (na.x * nb.x + na.y * nb.y + na.z * nb.z >= vb && na.x * nv.x + na.y * nv.y + na.z * nv.z >= vb)
        return true;
    return false;
}
