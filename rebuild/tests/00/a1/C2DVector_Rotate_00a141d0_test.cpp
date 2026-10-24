// Behaviour test for C2DVector::Rotate (0x00A141D0): the angle is a fraction of a full turn measured from +Y towards
// +X (atan2(X, Y) / 2pi, wrapped into [0, 1)); rotating adds the fraction and rebuilds X = sin, Y = cos, keeping the
// vector's length. A full turn returns the vector to itself.
#include <cstdio>
#include <math.h>

static int nearf(float a, float b) { float d = a - b; return d < 0.0005f && d > -0.0005f; }

inline float WrapAngle(float angle)
{
    if (angle < 0.0f || angle >= 1.0f) {
        angle = (float)fmod((double)angle, 1.0);
        if (angle < 0.0f)
            angle += 1.0f;
    }
    return angle;
}

class C2DVector {
public:
    float X, Y;
    float GetAngle() const { return WrapAngle((float)atan2(X, Y) * 0.15915494f); }
    float GetLength() const { return (float)sqrt(X * X + Y * Y); }
    void Rotate(float angle);
};

void C2DVector::Rotate(float angle)
{
    float newAngle = GetAngle() + angle;
    float length = GetLength();
    X = (float)sin(newAngle * 6.2831854820251465) * length;
    Y = (float)cos(newAngle * 6.2831854820251465) * length;
}

int main()
{
    int ok = 1;
    if (sizeof(C2DVector) != 8u) ok = 0;
    C2DVector v; v.X = 0.0f; v.Y = 1.0f;                 // angle 0
    v.Rotate(0.25f);                                      // quarter turn: +Y -> +X
    if (!(nearf(v.X, 1.0f) && nearf(v.Y, 0.0f))) ok = 0;
    v.X = 0.0f; v.Y = 2.0f;
    v.Rotate(-0.25f);                                     // back a quarter: +Y -> -X, length 2
    if (!(nearf(v.X, -2.0f) && nearf(v.Y, 0.0f))) ok = 0;
    v.X = -1.0f; v.Y = 0.0f;                              // atan2 gives -0.25, wrapped to 0.75
    v.Rotate(0.25f);                                      // 1.0 turn: points along +Y
    if (!(nearf(v.X, 0.0f) && nearf(v.Y, 1.0f))) ok = 0;
    v.X = 3.0f; v.Y = 4.0f;
    v.Rotate(1.0f);                                       // full turn keeps the vector
    if (!(nearf(v.X, 3.0f) && nearf(v.Y, 4.0f))) ok = 0;
    v.Rotate(0.5f);                                       // half turn negates it
    if (!(nearf(v.X, -3.0f) && nearf(v.Y, -4.0f))) ok = 0;
    if (!(nearf(WrapAngle(1.25f), 0.25f) && nearf(WrapAngle(-0.75f), 0.25f) && nearf(WrapAngle(0.5f), 0.5f))) ok = 0;

    if (!ok) { std::printf("00a141d0_TEST FAIL\n"); return 1; }
    std::printf("00a141d0_TEST PASS\n");
    return 0;
}