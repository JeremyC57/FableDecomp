// Behaviour test for 0x00A14040: C3DVector rotation through the angle helpers (angles are turn fractions in [0, 1)).
#include <cstdio>
#include <math.h>

static float WrapAngle(float angle)
{
    if (angle < 0.0f || angle >= 1.0f) {
        angle = (float)fmod((double)angle, 1.0);
        if (angle < 0.0f)
            angle += 1.0f;
    }
    return angle;
}
static const float TWO_PI = 6.2831855f;

class C3DVector {
public:
    float X, Y, Z;
    float GetAngleXY() const { return WrapAngle((float)atan2(X, Y) * 0.15915494f); }
    float GetAngleYZ() const { return WrapAngle((float)atan2(Z, sqrt(Y * Y + X * X)) * 0.15915494f); }
    float GetMagnitude() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
    void InitialiseFromAngles(float angleXY, float angleYZ, float length)
    {
        float cosYZ = (float)cos(angleYZ * (double)TWO_PI);
        X = (float)(sin(angleXY * (double)TWO_PI) * (cosYZ * length));
        Y = (float)(cos(angleXY * (double)TWO_PI) * (cosYZ * length));
        Z = (float)(sin(angleYZ * (double)TWO_PI) * length);
    }
    void RotateXY(float angle)
    {
        float angleXY = GetAngleXY() + angle;
        float angleYZ = GetAngleYZ();
        angleXY = WrapAngle(angleXY);
        InitialiseFromAngles(angleXY, angleYZ, GetMagnitude());
    }
    void Rotate(float angleXY, float angleYZ)
    {
        float newXY = GetAngleXY() + angleXY;
        float newYZ = GetAngleYZ() + angleYZ;
        newXY = WrapAngle(newXY);
        newYZ = WrapAngle(newYZ);
        InitialiseFromAngles(newXY, newYZ, GetMagnitude());
    }
};

static int Near(float a, float b) { return fabs(a - b) < 0.001f; }

int main()
{
    C3DVector v;
    v.X = 0.0f; v.Y = 2.0f; v.Z = 0.0f;              // heading 0 (along +Y)
    v.RotateXY(0.25f);                               // a quarter turn: along +X, length kept
    if (!Near(v.X, 2.0f) || !Near(v.Y, 0.0f) || !Near(v.Z, 0.0f)) { std::printf("00a14040_TEST FAIL\n"); return 1; }
    v.RotateXY(-0.5f);                               // wraps below zero: along -X
    if (!Near(v.X, -2.0f) || !Near(v.Y, 0.0f)) { std::printf("00a14040_TEST FAIL\n"); return 1; }
    v.X = 3.0f; v.Y = 0.0f; v.Z = 4.0f;              // the elevation and magnitude survive
    v.RotateXY(0.25f);
    if (!Near(v.X, 0.0f) || !Near(v.Y, -3.0f) || !Near(v.Z, 4.0f) || !Near(v.GetMagnitude(), 5.0f)) { std::printf("00a14040_TEST FAIL\n"); return 1; }
    std::printf("00a14040_TEST PASS\n");
    return 0;
}