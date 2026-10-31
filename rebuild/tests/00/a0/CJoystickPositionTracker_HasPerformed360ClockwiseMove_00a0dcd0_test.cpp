// Test: CJoystickPositionTracker::HasPerformed360ClockwiseMove -- walk 30 recent stick positions; false on a dead-zone
// position or a step turning more than 110 degrees; true once the summed turn exceeds 290 degrees.
#include <cstdio>
#include <cmath>
class C2DVector { public: float X, Y; float GetLength() const { return (float)sqrt(X * X + Y * Y); } };
static bool HasPerformed360(const C2DVector* recent)
{
    float minCosine = (float)cos(1.9198622098128482);
    C2DVector previous = recent[0];
    float totalTurns = 0.0f;
    for (int i = 1; i < 30; ++i) {
        const C2DVector& position = recent[i];
        if (!(position.GetLength() > 0.15f))
            return false;
        float cosine = previous.X * position.X;
        cosine += previous.Y * position.Y;
        if (cosine < minCosine)
            return false;
        totalTurns += (float)acos(cosine) * 0.15915494f;
        if (totalTurns > 0.80555558f)
            return true;
        previous = position;
    }
    return false;
}
int main()
{
    C2DVector circle[30], still[30], dead[30];
    for (int i = 0; i < 30; ++i) {
        double a = -i * (2.0 * 3.14159265358979 / 25.0);          // clockwise, 14.4 degrees a step
        circle[i].X = (float)cos(a); circle[i].Y = (float)sin(a);
        still[i].X = 1.0f; still[i].Y = 0.0f;
        dead[i] = circle[i];
    }
    dead[5].X = 0.1f; dead[5].Y = 0.0f;
    C2DVector jump[30];
    for (int i = 0; i < 30; ++i) jump[i] = still[i];
    jump[3].X = -1.0f;                                          // 180 degree step
    if (!HasPerformed360(circle) || HasPerformed360(still) || HasPerformed360(dead) || HasPerformed360(jump)) {
        std::printf("00a0dcd0_TEST FAIL\n"); return 1;
    }
    std::printf("00a0dcd0_TEST PASS\n");
    return 0;
}