// Behaviour test for 0x006A7930 CThingFilter_InMap::operator()
#include <cstdio>
struct C3DVector { float x, y, z; };
struct CTCPhysicsBase { C3DVector m_position; };
struct CThing { CTCPhysicsBase* PhysicsTC; };
struct CWorldMap {
    virtual long GetMapNumberAt(const C3DVector* pos) { return pos->x < 100.0f ? 1 : 2; }
};
struct CThingFilter_InMap {
    CWorldMap* WorldMap;
    long       MapNo;
    bool operator()(CThing* thing) const
    {
        CTCPhysicsBase* physics = thing->PhysicsTC;
        if (physics)
            return WorldMap->GetMapNumberAt(&physics->m_position) == MapNo;
        return false;
    }
};
int main()
{
    CWorldMap w; CTCPhysicsBase p; p.m_position.x = 50.0f; p.m_position.y = 0.0f; p.m_position.z = 0.0f;
    CThing t; t.PhysicsTC = 0;
    CThingFilter_InMap f; f.WorldMap = &w; f.MapNo = 1;
    bool ok = !f(&t);
    t.PhysicsTC = &p; if (!f(&t)) ok = false;
    p.m_position.x = 150.0f; if (f(&t)) ok = false;
    if (!ok) { std::printf("006a7930_TEST FAIL\n"); return 1; }
    std::printf("006a7930_TEST PASS\n");
    return 0;
}
