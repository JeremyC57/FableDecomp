#include <cstdio>
// mock Atan2 / rotations record their inputs so the data flow can be checked
static float Atan2(float x, float y) { return x + 10.0f * y; }
struct C3DVector { float X, Y, Z; };
struct CMatrix3x4 {
    C3DVector Right, Up, Forward, Position;
    void RotateA(float angle) { Forward.Z += angle; Forward.Y = 1.0f; }
    void RotateB(float angle) { Right.X = angle; Right.Y = -2.0f; }
    void GetRotationAngles(float& first, float& second, float& third) const
    {
        CMatrix3x4 m = *this;
        first = Atan2(m.Forward.Z, -m.Forward.X);
        m.RotateA(-first);
        second = Atan2(m.Forward.Z, m.Forward.Y);
        m.RotateB(-second);
        third = Atan2(m.Right.X, -m.Right.Y);
    }
};
int main()
{
    CMatrix3x4 m = { { 0, 0, 0 }, { 0, 0, 0 }, { 1.0f, 0.0f, 3.0f }, { 0, 0, 0 } };
    float a, b, c;
    m.GetRotationAngles(a, b, c);
    // first = 3 + 10*(-1) = -7; Forward.Z = 3 + 7 = 10, Forward.Y = 1 -> second = 10 + 10 = 20
    // Right.X = -20, Right.Y = -2 -> third = -20 + 10*2 = 0; the original is untouched
    if (a != -7.0f || b != 20.0f || c != 0.0f || m.Forward.Z != 3.0f) { std::printf("00c1cc10_TEST FAIL\n"); return 1; }
    std::printf("00c1cc10_TEST PASS\n");
    return 0;
}
