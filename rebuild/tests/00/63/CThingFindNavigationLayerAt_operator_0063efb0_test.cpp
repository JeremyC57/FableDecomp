// Behaviour test for 0x0063EFB0 CThingFindNavigationLayerAt::operator()
#include <cstdio>
struct C3DVector { float x, y, z; };
static int g_live = 0;
struct CPhysicsMeshObject {
    long m_level;
    CPhysicsMeshObject(long level) : m_level(level) { ++g_live; }
    CPhysicsMeshObject(const CPhysicsMeshObject& o) : m_level(o.m_level) { ++g_live; }
    ~CPhysicsMeshObject() { --g_live; }
    long GetNavigationLevelAt(const C3DVector& pos) { return m_level + (pos.x > 0.0f ? 1 : 0); }
};
struct CTCPhysicsBase {
    bool m_hasMesh; long m_level;
    virtual bool HasPhysicsMeshObject() { return m_hasMesh; }
    CPhysicsMeshObject GetPhysicsMeshObject() { return CPhysicsMeshObject(m_level); }
};
struct CThing { CTCPhysicsBase* PhysicsTC; };
struct CThingFindNavigationLayerAt {
    C3DVector Pos;
    long      Layer;
    bool operator()(CThing* thing)
    {
        CTCPhysicsBase* physics = thing->PhysicsTC;
        if (physics && physics->HasPhysicsMeshObject()) {
            CPhysicsMeshObject mesh = physics->GetPhysicsMeshObject();
            long level = mesh.GetNavigationLevelAt(Pos);
            if (level > Layer)
                Layer = level;
        }
        return true;
    }
};
int main()
{
    CTCPhysicsBase p; p.m_hasMesh = false; p.m_level = 4;
    CThing t; t.PhysicsTC = 0;
    CThingFindNavigationLayerAt f; f.Pos.x = 1.0f; f.Pos.y = 0.0f; f.Pos.z = 0.0f; f.Layer = 2;
    bool ok = f(&t) && f.Layer == 2;
    t.PhysicsTC = &p; if (!f(&t) || f.Layer != 2) ok = false;   // no mesh
    p.m_hasMesh = true; if (!f(&t) || f.Layer != 5) ok = false; // raised to 4+1
    p.m_level = 1; if (!f(&t) || f.Layer != 5) ok = false;      // lower level keeps the max
    if (g_live != 0) ok = false;
    if (!ok) { std::printf("0063efb0_TEST FAIL\n"); return 1; }
    std::printf("0063efb0_TEST PASS\n");
    return 0;
}