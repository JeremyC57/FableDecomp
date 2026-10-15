#include <cstdio>
struct C3DVector { float X, Y, Z; };
struct CThingPhysicsMeshInfo { int dummy; };
static CThingPhysicsMeshInfo g_info;
static int g_sphereCalls = 0;
struct CTCPhysicsBase {
    bool hasMesh;
    virtual ~CTCPhysicsBase() { }
    virtual CThingPhysicsMeshInfo* GetPhysicsMeshInfo() { return hasMesh ? &g_info : 0; }
    bool HasSphereSphereCollidedAt(CTCPhysicsBase* other, const C3DVector& pos, const C3DVector& otherPos) { ++g_sphereCalls; return pos.X == otherPos.X; }
    bool HasCollidedWithThingAt(CTCPhysicsBase* other, const C3DVector& pos, const C3DVector& otherPos)
    {
        if (other->GetPhysicsMeshInfo())
            return false;
        return HasSphereSphereCollidedAt(other, pos, otherPos);
    }
};
int main()
{
    CTCPhysicsBase a, b; a.hasMesh = false; b.hasMesh = true;
    C3DVector p; p.X = 1.0f; p.Y = 0.0f; p.Z = 0.0f;
    if (a.HasCollidedWithThingAt(&b, p, p) || g_sphereCalls != 0) { std::printf("00880800_TEST FAIL\n"); return 1; }
    b.hasMesh = false;
    if (!a.HasCollidedWithThingAt(&b, p, p) || g_sphereCalls != 1) { std::printf("00880800_TEST FAIL\n"); return 1; }
    std::printf("00880800_TEST PASS\n");
    return 0;
}
