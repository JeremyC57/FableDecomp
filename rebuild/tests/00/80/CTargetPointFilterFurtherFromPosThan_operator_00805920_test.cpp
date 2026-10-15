#include <cstdio>
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float MagnitudeSquared() const { return X * X + (Y * Y + Z * Z); }
};
struct CTargetPoint { void* p0; void* p1; long Index; C3DVector TargetingPosition; long FrameRejected; CTargetPoint(float x, float y, float z) : TargetingPosition(x, y, z) { } };
struct CTargetPointFilter_FurtherFromPosThan {
    const C3DVector* Pos; float Range;
    bool operator()(const CTargetPoint& point) const
    {
        if ((point.TargetingPosition - *Pos).MagnitudeSquared() > Range * Range) return true;
        return false;
    }
};
int main()
{
    C3DVector origin(1.0f, 1.0f, 1.0f);
    CTargetPointFilter_FurtherFromPosThan f; f.Pos = &origin; f.Range = 5.0f;
    bool ok = f(CTargetPoint(7.0f, 1.0f, 1.0f)) && !f(CTargetPoint(4.0f, 5.0f, 1.0f)) && !f(CTargetPoint(1.0f, 1.0f, 1.0f)) && f(CTargetPoint(-3.0f, -3.0f, -3.0f));
    if (!ok) { std::printf("00805920_TEST FAIL\n"); return 1; }
    std::printf("00805920_TEST PASS\n");
    return 0;
}