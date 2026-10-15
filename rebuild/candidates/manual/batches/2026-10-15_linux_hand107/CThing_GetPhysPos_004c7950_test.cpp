// Behaviour test for CThing::GetPhysPos @ 0x004C7950: physics position plus the 2D offset in x/y, z unchanged.
#include <cstdio>
struct C2DVector { float X, Y; C2DVector() { } C2DVector(float x, float y) : X(x), Y(y) { } };
struct C3DVector { float X, Y, Z; C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { } };
inline C3DVector operator+(const C2DVector& offset, const C3DVector& position)
{
    return C3DVector(offset.X + position.X, offset.Y + position.Y, position.Z);
}
struct CTCPhysicsBase {
    C3DVector Position;
    CTCPhysicsBase() : Position(1.0f, 2.0f, 3.0f) { }
    virtual C2DVector GetPhysOffset2D() { return C2DVector(0.5f, -4.0f); }
};
struct CThing {
    CTCPhysicsBase* PhysicsTC;
    C3DVector GetPhysPos() const
    {
        CTCPhysicsBase* physics = PhysicsTC;
        return physics->GetPhysOffset2D() + physics->Position;
    }
};
int main()
{
    CTCPhysicsBase p; CThing t; t.PhysicsTC = &p;
    C3DVector r = t.GetPhysPos();
    if (r.X != 1.5f || r.Y != -2.0f || r.Z != 3.0f) { std::printf("004c7950_TEST FAIL\n"); return 1; }
    std::printf("004c7950_TEST PASS\n");
    return 0;
}
