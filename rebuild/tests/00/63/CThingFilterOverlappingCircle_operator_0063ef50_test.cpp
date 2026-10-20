// Behaviour test for 0063ef50: body copied from src/0063ef50.cpp (CThingFilter_OverlappingCircle::operator()), GetMapwhoPosition stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct C3DVector {
    float x, y, z;
    C3DVector() {}
};
static C3DVector g_pos; static int g_posCalls;
struct CTCPhysicsBase {
    unsigned char _pad00[0x34];
    float         m_radius;
    C3DVector GetMapwhoPosition() const { ++g_posCalls; return g_pos; }
};
struct CThing {
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;
};
struct CThingFilter_OverlappingCircle {
    C3DVector SpherePos;
    float     Radius;
    bool Overlaps(const C3DVector& pos, const CTCPhysicsBase* physics) const
    {
        float dx = pos.x - SpherePos.x;
        float dy = pos.y - SpherePos.y;
        float r = physics->m_radius + Radius;
        float d2 = dx * dx;
        d2 += dy * dy;
        if (d2 < r * r)
            return true;
        return false;
    }
    bool operator()(CThing* thing) const;
};
bool CThingFilter_OverlappingCircle::operator()(CThing* thing) const
{
    CTCPhysicsBase* physics = thing->PhysicsTC;
    if (physics)
        return Overlaps(physics->GetMapwhoPosition(), physics);
    return false;
}
int main()
{
    CThingFilter_OverlappingCircle f;
    f.SpherePos.x = 10.0f; f.SpherePos.y = 20.0f; f.SpherePos.z = 0.0f; f.Radius = 2.0f;
    CThing thing; memset(&thing, 0, sizeof(thing));
    CTCPhysicsBase phys; memset(&phys, 0, sizeof(phys)); phys.m_radius = 1.0f;
    // 1. no physics: false, no position query
    g_posCalls = 0; CHECK(f(&thing) == false); CHECK(g_posCalls == 0);
    thing.PhysicsTC = &phys;
    // 2. overlapping (distance 2.5 < r 3): true
    g_pos.x = 12.5f; g_pos.y = 20.0f; g_pos.z = 99.0f; g_posCalls = 0;
    CHECK(f(&thing) == true); CHECK(g_posCalls == 1);
    // 3. touching exactly (distance 3 == r 3): false (strict less-than)
    g_pos.x = 10.0f; g_pos.y = 23.0f; CHECK(f(&thing) == false);
    // 4. far away (dx 3, dy 4 -> 5 > 3): false; z ignored
    g_pos.x = 13.0f; g_pos.y = 24.0f; g_pos.z = 0.0f; CHECK(f(&thing) == false);
    // 5. dy only contributes: dx 0, dy 2.9 < 3
    g_pos.x = 10.0f; g_pos.y = 17.1f; CHECK(f(&thing) == true);
    if (fails == 0) printf("0063ef50_TEST PASS\n");
    else printf("0063ef50_TEST FAIL (%d)\n", fails);
    return fails;
}