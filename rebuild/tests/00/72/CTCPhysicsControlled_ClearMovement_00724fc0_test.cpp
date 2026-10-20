// Behaviour test for 00724fc0: body copied from src/00724fc0.cpp, callees stubbed.
#include <cstdio>
#include <cstring>
#include <new>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
template <class P> struct CAllocProxy { P p; CAllocProxy(P v) : p(v) { } operator P() const { return p; } CAllocProxy& operator=(P v) { p = v; return *this; } };
struct C3DVector {
    float X, Y, Z;
    void Set(float x, float y, float z) { Z = z; Y = y; X = x; }
    C3DVector& operator+=(const C3DVector& v) { X += v.X; Y += v.Y; Z += v.Z; return *this; }
};
struct CTCPhysicsBase { virtual void v00() {} void* m_tcBase[2]; C3DVector Position; C3DVector OldPosition; long OldPositionLastFrameSet; float Velocity_X, Velocity_Y, Velocity_Z; float Radius; void* PhysicsDef; unsigned char Flags3c[4]; };
struct CTCPhysicsStandard : CTCPhysicsBase { unsigned char _standard[0x48]; };
struct CTCPhysicsControlled : CTCPhysicsStandard {
    C3DVector ImpulseVelocity; C3DVector ForcedMoveToPos; bool ForcedMoveToPosSet : 1; bool ForcedMoveToPosUsePhysics : 1; float RenderZOffset; float LastRenderZOffset;
    void ClearMovement();
};
void CTCPhysicsControlled::ClearMovement()
{
    ImpulseVelocity.Set(0.0f, 0.0f, 0.0f);
    ForcedMoveToPosSet = false;
    ForcedMoveToPosUsePhysics = true;
    Velocity_X = Velocity_Y = Velocity_Z = 0.0f;
}
int main()
{
    CTCPhysicsControlled tc; memset(&tc.m_tcBase, 0x55, sizeof(tc) - sizeof(void*));
    tc.ImpulseVelocity.X = 1.0f; tc.ImpulseVelocity.Y = 2.0f; tc.ImpulseVelocity.Z = 3.0f; tc.Velocity_X = 4.0f; tc.Velocity_Y = 5.0f; tc.Velocity_Z = 6.0f;
    tc.ForcedMoveToPosSet = true; tc.ForcedMoveToPosUsePhysics = false;
    tc.ClearMovement();
    CHECK(tc.ImpulseVelocity.X == 0.0f && tc.ImpulseVelocity.Y == 0.0f && tc.ImpulseVelocity.Z == 0.0f);
    CHECK(tc.Velocity_X == 0.0f && tc.Velocity_Y == 0.0f && tc.Velocity_Z == 0.0f);
    CHECK(tc.ForcedMoveToPosSet == false && tc.ForcedMoveToPosUsePhysics == true);
    CHECK(*(unsigned long*)&tc.ForcedMoveToPos.X == 0x55555555 && *(unsigned long*)&tc.Radius == 0x55555555 && *(unsigned long*)&tc.RenderZOffset == 0x55555555);
    if (fails == 0) printf("00724fc0_TEST PASS\n"); else printf("00724fc0_TEST FAIL\n");
    return fails;
}