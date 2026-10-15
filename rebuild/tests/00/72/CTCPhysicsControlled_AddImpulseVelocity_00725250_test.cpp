#include <cstdio>
struct C3DVector {
    float X, Y, Z;
    C3DVector& operator+=(const C3DVector& v) { X += v.X; Y += v.Y; Z += v.Z; return *this; }
};
struct CTCPhysicsControlled {
    C3DVector ImpulseVelocity;
    C3DVector ForcedMoveToPos;
    void AddImpulseVelocity(const C3DVector& v) { ImpulseVelocity += v; }
};
int main()
{
    CTCPhysicsControlled c;
    c.ImpulseVelocity.X = 1.0f; c.ImpulseVelocity.Y = 2.0f; c.ImpulseVelocity.Z = 3.0f;
    c.ForcedMoveToPos.X = c.ForcedMoveToPos.Y = c.ForcedMoveToPos.Z = 9.0f;
    C3DVector d; d.X = 0.5f; d.Y = -2.0f; d.Z = 10.0f;
    c.AddImpulseVelocity(d);
    if (c.ImpulseVelocity.X != 1.5f || c.ImpulseVelocity.Y != 0.0f || c.ImpulseVelocity.Z != 13.0f || c.ForcedMoveToPos.X != 9.0f) { std::printf("00725250_TEST FAIL\n"); return 1; }
    std::printf("00725250_TEST PASS\n");
    return 0;
}