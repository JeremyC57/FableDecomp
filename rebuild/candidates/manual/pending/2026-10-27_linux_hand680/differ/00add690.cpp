// 00add690 C3DBoundingSphere::FastBuildFromPoints
// Grade: DIFFER(646v648) -- remainder loop of the 4x-unrolled squared-distance scan: retail copies the remaining count into esi (the unrolled loop's counter register, `sub ebx,edi ; mov esi,ebx`) and decrements esi; VC7.1 here decrements ebx in place (2 bytes shorter, `lea edx` scheduled one slot earlier). The rest matches.
// Next step: 12 shapes (index type and sign, reused/declared index, while form, const ref point, comparison spelling) leave it. VC7.1 decides this when it unrolls the loop; try the real C3DVector operator- / GetSquaredMagnitude header bodies or treat as an unroller register-coalescing lane gap.
// Leaf: ?FastBuildFromPoints@C3DBoundingSphere@@QAEXPBVC3DVector@@J@Z
// Flags:
// Layout: original C3DVector12 XYZ and sphere16 Center0 Radius12; no shared headers supplied.
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector&v):X(v.X),Y(v.Y),Z(v.Z){}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    const C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    const C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    const C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    float GetSquaredMagnitude() const { return X * X + Y * Y + Z * Z; }
};
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
class C3DBoundingSphere {
public:
    C3DVector Center;   // +0x00
    float Radius;       // +0x0c
    void FastBuildFromPoints(const C3DVector* points, long count);
};

void C3DBoundingSphere::FastBuildFromPoints(const C3DVector* points, long count)
{
    C3DVector minimum(0.0f, 0.0f, 0.0f);
    C3DVector maximum(0.0f, 0.0f, 0.0f);
    long i;
    if (count > 0)
    for (i = 0; i != count; ++i) {
        if (!(minimum.X < points[i].X)) minimum.X = points[i].X;
        if (!(minimum.Y < points[i].Y)) minimum.Y = points[i].Y;
        if (!(minimum.Z < points[i].Z)) minimum.Z = points[i].Z;
        if (!(maximum.X > points[i].X)) maximum.X = points[i].X;
        if (!(maximum.Y > points[i].Y)) maximum.Y = points[i].Y;
        if (!(maximum.Z > points[i].Z)) maximum.Z = points[i].Z;
    }
    Center = C3DVector((maximum.X + minimum.X) * 0.5f, (maximum.Y + minimum.Y) * 0.5f, (maximum.Z + minimum.Z) * 0.5f);
    float best = 0.0f;
    for (unsigned long j = 0; j < (unsigned long)count; ++j) {
        float d = (points[j] - Center).GetSquaredMagnitude();
        if (best < d) best = d;
    }
    Radius = (float)sqrt(best);
}
