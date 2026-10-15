// Behaviour test for 0x006A7A80 CThingFilter_IsInBuilding::operator()
#include <cstdio>
struct C3DVector { float x, y, z; };
struct CThing {
    C3DVector m_pos;
    const C3DVector& GetPosition() const { return m_pos; }
};
struct CThingBuilding {
    float minX, maxX;
    bool IsPointInside(const C3DVector& pos) { return pos.x >= minX && pos.x <= maxX; }
};
struct CThingFilter_IsInBuilding {
    CThingBuilding* Building;
    bool operator()(CThing* thing) const { return Building->IsPointInside(thing->GetPosition()); }
};
int main()
{
    CThingBuilding b; b.minX = 0.0f; b.maxX = 10.0f;
    CThing t; t.m_pos.x = 5.0f; t.m_pos.y = 0.0f; t.m_pos.z = 0.0f;
    CThingFilter_IsInBuilding f; f.Building = &b;
    bool ok = f(&t);
    t.m_pos.x = 11.0f; if (f(&t)) ok = false;
    if (!ok) { std::printf("006a7a80_TEST FAIL\n"); return 1; }
    std::printf("006a7a80_TEST PASS\n");
    return 0;
}
