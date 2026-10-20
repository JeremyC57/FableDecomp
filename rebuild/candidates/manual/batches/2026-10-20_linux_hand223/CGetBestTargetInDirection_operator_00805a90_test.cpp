#include <cstdio>
#include <cmath>

struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    void Normalise() { float l = (float)std::sqrt(X * X + Y * Y + Z * Z); X /= l; Y /= l; Z /= l; }
};
static float g_fTolerance = 1e-4f;
struct CTargetPoint { int pad[3]; C3DVector TargetingPosition; CTargetPoint(float x, float y) : TargetingPosition(x, y, 0) {} };
struct CGetBestTargetInDirection {
    const C3DVector* BasePos; C3DVector TestLine; float Direction; CTargetPoint* PBestTargetPoint; float CosThetaBestAngle;
    CGetBestTargetInDirection() : TestLine(0, 1, 0) {}
    void operator()(CTargetPoint* point)
    {
        C3DVector delta(point->TargetingPosition.X - BasePos->X, point->TargetingPosition.Y - BasePos->Y, 0.0f);
        float lengthSquared = delta.X * delta.X;
        lengthSquared += delta.Y * delta.Y;
        if (lengthSquared > g_fTolerance * g_fTolerance) delta.Normalise();
        float cosTheta = delta.Y * TestLine.Y;
        cosTheta += delta.Z * TestLine.Z;
        cosTheta += delta.X * TestLine.X;
        float side = delta.Y * TestLine.X - delta.X * TestLine.Y;
        if (Direction > 0.0f && side > 0.0f) {
            if (cosTheta > CosThetaBestAngle) { PBestTargetPoint = point; CosThetaBestAngle = cosTheta; }
        } else if (Direction <= 0.0f && side <= 0.0f) {
            if (cosTheta > CosThetaBestAngle) { PBestTargetPoint = point; CosThetaBestAngle = cosTheta; }
        }
    }
};
int main()
{
    C3DVector base(0, 0, 0);
    CTargetPoint left(-1.0f, 1.0f), right(1.0f, 1.0f), farRight(3.0f, 1.0f), nearRight(1.0f, 4.0f);
    CGetBestTargetInDirection f; f.BasePos = &base; f.Direction = -1.0f; f.PBestTargetPoint = 0; f.CosThetaBestAngle = -2.0f;
    f(&left); f(&farRight); f(&right); f(&nearRight);     // test line +Y: side = -x, Direction <= 0 keeps x >= 0
    if (f.PBestTargetPoint != &nearRight || f.CosThetaBestAngle < 0.97f) { std::printf("00805a90_TEST FAIL\n"); return 1; }
    CGetBestTargetInDirection g; g.BasePos = &base; g.Direction = 1.0f; g.PBestTargetPoint = 0; g.CosThetaBestAngle = -2.0f;
    g(&right); g(&farRight);
    if (g.PBestTargetPoint != 0) { std::printf("00805a90_TEST FAIL\n"); return 1; }
    g(&left);
    if (g.PBestTargetPoint != &left) { std::printf("00805a90_TEST FAIL\n"); return 1; }
    std::printf("00805a90_TEST PASS\n");
    return 0;
}
