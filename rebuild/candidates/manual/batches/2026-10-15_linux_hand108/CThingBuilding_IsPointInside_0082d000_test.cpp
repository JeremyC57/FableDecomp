#include <cstdio>
// behaviour test for CThingBuilding::IsPointInside (0x0082D000)
class CCharString { public: const char* text; };
struct C3DVector { float X, Y, Z; };
CCharString g_containmentVolumeName = { "inside" };
static int g_live = 0;
struct CPhysicsMeshObject {
    float minX, maxX;
    CPhysicsMeshObject(float a, float b) : minX(a), maxX(b) { ++g_live; }
    CPhysicsMeshObject(const CPhysicsMeshObject& o) : minX(o.minX), maxX(o.maxX) { ++g_live; }
    ~CPhysicsMeshObject() { --g_live; }
    bool IsInContainmentVolume(const CCharString& volume, const C3DVector& pos, float tolerance)
    {
        return volume.text == g_containmentVolumeName.text && tolerance == 0.0f && pos.X >= minX && pos.X <= maxX;
    }
};
struct CTCPhysicsBase {
    const void* mesh;
    virtual const void* GetPhysicsMesh() { return mesh; }
    CPhysicsMeshObject GetPhysicsMeshObject() { return CPhysicsMeshObject(0.0f, 10.0f); }
};
struct CThingBuilding {
    CTCPhysicsBase* m_pPhysics;
    bool IsPointInside(const C3DVector& pos);
};
bool CThingBuilding::IsPointInside(const C3DVector& pos)
{
    CTCPhysicsBase* physics = m_pPhysics;
    if (physics && physics->GetPhysicsMesh()) {
        CPhysicsMeshObject mesh = m_pPhysics->GetPhysicsMeshObject();
        return mesh.IsInContainmentVolume(g_containmentVolumeName, pos, 0.0f);
    }
    return false;
}
int main()
{
    CThingBuilding b; CTCPhysicsBase phys; int meshTag = 0;
    C3DVector in = { 5.0f, 0.0f, 0.0f }, out = { 20.0f, 0.0f, 0.0f };
    b.m_pPhysics = 0;
    if (b.IsPointInside(in)) { std::printf("0082d000_TEST FAIL\n"); return 1; }
    b.m_pPhysics = &phys; phys.mesh = 0;
    if (b.IsPointInside(in)) { std::printf("0082d000_TEST FAIL\n"); return 1; }
    phys.mesh = &meshTag;
    if (!b.IsPointInside(in) || b.IsPointInside(out) || g_live != 0) { std::printf("0082d000_TEST FAIL\n"); return 1; }
    std::printf("0082d000_TEST PASS\n");
    return 0;
}
