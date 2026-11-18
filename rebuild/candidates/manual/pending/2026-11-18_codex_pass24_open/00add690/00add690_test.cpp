#include <math.h>
#include <stdio.h>

struct C3DVector {
    float X, Y, Z;
    C3DVector(float x = 0, float y = 0, float z = 0) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& other) const { return C3DVector(X-other.X,Y-other.Y,Z-other.Z); }
    float GetSquaredMagnitude() const { return X*X + Y*Y + Z*Z; }
};
struct C3DBoundingSphere {
    C3DVector Center;
    float Radius;
    void FastBuildFromPoints(const C3DVector*, long);
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

int main()
{
    const C3DVector points[] = {C3DVector(1,1,1),C3DVector(2,4,6),C3DVector(0,1,3),C3DVector(1,0,3),C3DVector(-6,-4,-2)};
    C3DBoundingSphere empty, small, full;
    empty.FastBuildFromPoints(0, 0);
    small.FastBuildFromPoints(points, 4);
    full.FastBuildFromPoints(points, 5);
    const bool ok = empty.Center.X == 0 && empty.Center.Y == 0 && empty.Center.Z == 0 && empty.Radius == 0
        && small.Center.X == 1 && small.Center.Y == 2 && small.Center.Z == 3
        && fabs(small.Radius*small.Radius - 14.0f) < 0.001f
        && full.Center.X == -2 && full.Center.Y == 0 && full.Center.Z == 2
        && fabs(full.Radius*full.Radius - 48.0f) < 0.001f;
    puts(ok ? "00add690_TEST PASS" : "00add690_TEST FAIL");
    return ok ? 0 : 1;
}
