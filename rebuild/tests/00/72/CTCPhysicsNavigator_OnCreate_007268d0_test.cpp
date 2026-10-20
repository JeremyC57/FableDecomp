// Behaviour test for 007268d0: body copied from src/007268d0.cpp, callees stubbed.
#include <cstdio>
#include <cstring>
#include <new>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
template <class P> struct CAllocProxy { P p; CAllocProxy(P v) : p(v) { } operator P() const { return p; } CAllocProxy& operator=(P v) { p = v; return *this; } };
static int g_stdCreate = 0;
struct C3DVector { float X, Y, Z; void Set(float x, float y, float z) { Z = z; Y = y; X = x; } };
struct CRightHandedSet { C3DVector Up; C3DVector Forward; };
struct CTCPhysicsBase {
    virtual void v00() {}
    void* m_tcBase[2]; C3DVector Position; C3DVector OldPosition; long OldPositionLastFrameSet; C3DVector Velocity; float Radius; void* PhysicsDef;
    bool Moving : 1; bool OldMoving : 1;
};
struct CTCPhysicsStandard : CTCPhysicsBase {
    void* PPhysicsMeshInfo; C3DVector OffsetVector3D; CRightHandedSet RHSet; CRightHandedSet OldRHSet; long RHSetLastFrameSet; unsigned char m_flags84, m_flags85;
    void OnCreate() { ++g_stdCreate; }
};
#pragma pack(push, 1)
struct CTCPhysicsNavigator : CTCPhysicsStandard {
    bool Navigating; C3DVector PositionToNavigateTo; C3DVector AdditionalImpulse; C3DVector ForcedMoveToPos; bool ForcedMoveToPosSet : 1; bool ForcedMoveToPosUsePhysics : 1;
    void OnCreate();
};
#pragma pack(pop)
void CTCPhysicsNavigator::OnCreate()
{
    CTCPhysicsStandard::OnCreate();
    PositionToNavigateTo = Position;
    ForcedMoveToPosSet = false;
    Navigating = true;
    AdditionalImpulse.Set(0.0f, 0.0f, 0.0f);
}
int main()
{
    CTCPhysicsNavigator tc; memset(&tc.m_tcBase, 0x55, sizeof(tc) - sizeof(void*));
    tc.Position.X = 1.5f; tc.Position.Y = 2.5f; tc.Position.Z = 3.5f; tc.AdditionalImpulse.X = 9.0f; tc.AdditionalImpulse.Y = 9.0f; tc.AdditionalImpulse.Z = 9.0f;
    tc.ForcedMoveToPosSet = true; tc.ForcedMoveToPosUsePhysics = true; tc.Navigating = false;
    tc.OnCreate();
    CHECK(g_stdCreate == 1);
    CHECK(tc.PositionToNavigateTo.X == 1.5f && tc.PositionToNavigateTo.Y == 2.5f && tc.PositionToNavigateTo.Z == 3.5f);
    CHECK(tc.ForcedMoveToPosSet == false && tc.ForcedMoveToPosUsePhysics == true && tc.Navigating == true);
    CHECK(tc.AdditionalImpulse.X == 0.0f && tc.AdditionalImpulse.Y == 0.0f && tc.AdditionalImpulse.Z == 0.0f);
    CHECK(*(unsigned long*)&tc.ForcedMoveToPos.X == 0x55555555 && *(unsigned long*)&tc.Velocity.X == 0x55555555);
    if (fails == 0) printf("007268d0_TEST PASS\n"); else printf("007268d0_TEST FAIL\n");
    return fails;
}