// C3DBoundingSphere::FastBuildFromPoints(const C3DVector*, long) -- the box centre of the points (min/max from zero) as Center, the farthest point's distance as Radius
// Grade: DIFFER(646v648) -- the 4x-unrolled radius loop's remainder: retail copies count into esi (`mov esi,ebx ; dec esi`) and places the base lea after it, ours counts ebx down directly; everything else relocation-equal
// Leaf: FastBuildFromPoints
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
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
        float d = (points[j] - Center).GetLengthSquared();
        if (best < d) best = d;
    }
    Radius = (float)sqrt(best);
}
