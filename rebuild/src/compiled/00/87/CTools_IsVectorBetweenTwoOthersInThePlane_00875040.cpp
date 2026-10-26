// CTools::IsVectorBetweenTwoOthersInThePlane(const C3DVector& a, const C3DVector& b, const C3DVector& v) -- with all three directions normalised: true when v is at least as close in angle to a as b is, i.e. a.b >= v.b and a.v >= v.b
// Leaf: IsVectorBetweenTwoOthersInThePlane
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00875040, length 220 bytes (`ret 0Ch`, `this` unused; 224 with padding).
// Callees: C3DVector::AccurateNormalise @ 0x00A14440 (x3, on frame copies).
// Shape: each direction is bound to a const reference to an inline Normalised() copy (a named copy normalised in
// place and returned by value); the reference-bound temporaries get retail's frame slots (b at +0, v at +0xc, a at +0x18),
// named copies swap a and v. The v.b dot product is a named float kept on the FPU across both compares (`>=`);
// each dot product sums x, y, z in source (VC emits z first).
class C3DVector { public:
    float x, y, z;
    C3DVector(const C3DVector& o) : x(o.x), y(o.y), z(o.z) { }
    C3DVector& AccurateNormalise();                                       // 0x00A14440 (Ego_r ?AccurateNormalise@C3DVector@@QAEAAV1@XZ)
    C3DVector Normalised() const { C3DVector r(*this); r.AccurateNormalise(); return r; }
};

struct CTools {
    bool IsVectorBetweenTwoOthersInThePlane(const C3DVector& a, const C3DVector& b, const C3DVector& v);
};

bool CTools::IsVectorBetweenTwoOthersInThePlane(const C3DVector& a, const C3DVector& b, const C3DVector& v)
{
    const C3DVector& na = a.Normalised();
    const C3DVector& nb = b.Normalised();
    const C3DVector& nv = v.Normalised();
    float vb = nb.x * nv.x + nb.y * nv.y + nb.z * nv.z;
    if (na.x * nb.x + na.y * nb.y + na.z * nb.z >= vb && na.x * nv.x + na.y * nv.y + na.z * nv.z >= vb)
        return true;
    return false;
}