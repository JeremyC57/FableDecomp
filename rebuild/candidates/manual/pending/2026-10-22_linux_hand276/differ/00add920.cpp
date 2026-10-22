// C3DBoundingSphere::AccurateBuildFromPoints(const C3DVector*, long, float) -- refine the fast sphere: step the centre toward the farthest point with a halving step until it drops below the accuracy, keeping the best centre/radius
// Grade: DIFFER(637v690) -- first draft: retail keeps this in ecx (no esi copy), tail-duplicates the centre step into both arms of the best-radius test with separate frame temps (frame 0x40 vs 0x28)
// Leaf: AccurateBuildFromPoints
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector& operator+=(const C3DVector& o) { X += o.X; Y += o.Y; Z += o.Z; return *this; }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
class C3DBoundingSphere {
public:
    C3DVector Center;   // +0x00
    float Radius;       // +0x0c
    void FastBuildFromPoints(const C3DVector* points, long count);                 // 0x00ADD690
    void AccurateBuildFromPoints(const C3DVector* points, long count, float accuracy);
};

void C3DBoundingSphere::AccurateBuildFromPoints(const C3DVector* points, long count, float accuracy)
{
    FastBuildFromPoints(points, count);
    if (count == 0)
        return;
    float bestRadiusSquared = 1e30f;
    C3DVector bestCenter = Center;
    for (float step = 0.25f; step > accuracy; step *= 0.5) {
        float farthest = 0.0f;
        unsigned long farthestIndex = 0;
        for (unsigned long i = 0; i < (unsigned long)count; ++i) {
            float d = (points[i] - Center).GetLengthSquared();
            if (d > farthest) {
                farthest = d;
                farthestIndex = i;
            }
        }
        if (farthest < bestRadiusSquared) {
            bestRadiusSquared = farthest;
            bestCenter = Center;
        }
        Center += (points[farthestIndex] - Center) * step;
    }
    Center = bestCenter;
    Radius = (float)sqrt(bestRadiusSquared);
}
