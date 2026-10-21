// test for 0066f9b0 CTCPhysicsBase::SetVelocity
#include <stdio.h>
#include <string.h>
class C3DVector {
public:
    float X, Y, Z;
};
class CTCPhysicsBase {
public:
    void SetAsMoving(bool moving);                       // 0x006B0130
    void SetVelocity(const C3DVector& velocity);
    unsigned char _pad00[0x28];                          // +0x00 vptr, CTCBase, Position, OldPosition, ...
    C3DVector     Velocity;                              // +0x28
};
void CTCPhysicsBase::SetVelocity(const C3DVector& velocity)
{
    Velocity = velocity;
    SetAsMoving(true);
}

static int g_moving = -1; static void* g_this;
void CTCPhysicsBase::SetAsMoving(bool moving) { g_moving = moving; g_this = this; }
int main()
{
    CTCPhysicsBase b; memset(&b, 0, sizeof(b));
    C3DVector v = { 1.0f, -2.0f, 3.5f };
    b.SetVelocity(v);
    int ok = b.Velocity.X == 1.0f && b.Velocity.Y == -2.0f && b.Velocity.Z == 3.5f && g_moving == 1 && g_this == &b
          && (char*)&b.Velocity - (char*)&b == 0x28;
    printf(ok ? "0066f9b0_TEST PASS\n" : "0066f9b0_TEST FAIL\n");
    return ok ? 0 : 1;
}