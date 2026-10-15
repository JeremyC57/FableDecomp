#include <cstdio>
struct C3DVector {
    float X, Y, Z;
    C3DVector& operator+=(const C3DVector& v) { X += v.X; Y += v.Y; Z += v.Z; return *this; }
};
struct CTCPhysicsNavigator {
    C3DVector PositionToNavigateTo;
    C3DVector AdditionalImpulse;
    void AddImpulse(const C3DVector& impulse) { AdditionalImpulse += impulse; }
};
int main()
{
    CTCPhysicsNavigator n; n.AdditionalImpulse.X = 1.0f; n.AdditionalImpulse.Y = 2.0f; n.AdditionalImpulse.Z = 3.0f;
    n.PositionToNavigateTo.X = 5.0f;
    C3DVector i; i.X = 1.0f; i.Y = 1.0f; i.Z = -3.0f;
    n.AddImpulse(i); n.AddImpulse(i);
    if (n.AdditionalImpulse.X != 3.0f || n.AdditionalImpulse.Y != 4.0f || n.AdditionalImpulse.Z != -3.0f || n.PositionToNavigateTo.X != 5.0f) { std::printf("00726780_TEST FAIL\n"); return 1; }
    std::printf("00726780_TEST PASS\n");
    return 0;
}