// C3DBoundingSphere::AccurateBuildFromPoints(const C3DVector*, long, float) -- refine the fast sphere: step the centre toward the farthest point with a halving step until it drops below the accuracy, keeping the best centre/radius
//   reads this only through ecx, so VC7.1's same-TU callee analysis keeps `this` in ecx across the call, as retail); the inner
//   search is a 4x hand-unrolled do/while under `count >= 4` plus a remainder loop with one `float d` per outer iteration (d
//   homes in the dead `count` argument slot); the outer loop is `if (step > accuracy) do { ... } while (step > accuracy)` with the
//   centre step written out in BOTH arms of the best-radius test and `step *= 0.5` (double) only in the else arm; bestCenter is
//   copied memberwise.
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

__declspec(noinline) void C3DBoundingSphere::FastBuildFromPoints(const C3DVector* points, long count)
{
    C3DVector sum(0.0f, 0.0f, 0.0f);
    for (long i = 0; i < count; ++i)
        sum += points[i];
    float inv = count ? 1.0f / (float)count : 0.0f;
    Center = sum * inv;
    float radiusSquared = 0.0f;
    for (long j = 0; j < count; ++j) {
        float d = (points[j] - Center).GetLengthSquared();
        if (d > radiusSquared)
            radiusSquared = d;
    }
    Radius = (float)sqrt(radiusSquared);
}
void C3DBoundingSphere::AccurateBuildFromPoints(const C3DVector* points, long count, float accuracy)
{
    FastBuildFromPoints(points, count);
    if (count == 0)
        return;
    float bestRadiusSquared = 1e30f;
    C3DVector bestCenter;
    bestCenter.X = Center.X;
    bestCenter.Y = Center.Y;
    bestCenter.Z = Center.Z;
    float step = 0.25f;
    if (step > accuracy) do {
        float d;
        float farthest = 0.0f;
        unsigned long farthestIndex = 0;
        unsigned long i = 0;
        if (count >= 4) {
            do {
                d = (points[i] - Center).GetLengthSquared();
                if (d > farthest) { farthest = d; farthestIndex = i; }
                d = (points[i + 1] - Center).GetLengthSquared();
                if (d > farthest) { farthest = d; farthestIndex = i + 1; }
                d = (points[i + 2] - Center).GetLengthSquared();
                if (d > farthest) { farthest = d; farthestIndex = i + 2; }
                d = (points[i + 3] - Center).GetLengthSquared();
                if (d > farthest) { farthest = d; farthestIndex = i + 3; }
                i += 4;
            } while (i < (unsigned long)(count - 3));
        }
        for (; i < (unsigned long)count; ++i) {
            d = (points[i] - Center).GetLengthSquared();
            if (d > farthest) { farthest = d; farthestIndex = i; }
        }
        if (farthest < bestRadiusSquared) {
            bestRadiusSquared = farthest;
            bestCenter = Center;
            Center += (points[farthestIndex] - Center) * step;
        } else {
            Center += (points[farthestIndex] - Center) * step;
            step *= 0.5;
        }
    } while (step > accuracy);
    Center = bestCenter;
    Radius = (float)sqrt(bestRadiusSquared);
}