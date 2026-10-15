// Behaviour test for 0x006A7780 CThingFilter_In3DRange::operator()
#include <cstdio>
struct C3DVector { float x, y, z; };
struct CTCPhysicsBase { C3DVector m_position; };
struct CThing { CTCPhysicsBase* PhysicsTC; };
struct CThingFilter_In3DRange {
    C3DVector Pos;
    float     Range;
    bool operator()(CThing* thing) const
    {
        CTCPhysicsBase* physics = thing->PhysicsTC;
        if (physics) {
            float range = Range;
            float dx = Pos.x - physics->m_position.x;
            float dy = Pos.y - physics->m_position.y;
            float dz = Pos.z - physics->m_position.z;
            return dx * dx + dy * dy + dz * dz < range * range;
        }
        return false;
    }
};
int main()
{
    CTCPhysicsBase p; p.m_position.x = 2.0f; p.m_position.y = 3.0f; p.m_position.z = 6.0f;   // distance 7
    CThing t; t.PhysicsTC = 0;
    CThingFilter_In3DRange f; f.Pos.x = 0.0f; f.Pos.y = 0.0f; f.Pos.z = 0.0f; f.Range = 7.5f;
    bool ok = !f(&t);
    t.PhysicsTC = &p;
    if (!f(&t)) ok = false;
    f.Range = 7.0f; if (f(&t)) ok = false;
    if (!ok) { std::printf("006a7780_TEST FAIL\n"); return 1; }
    std::printf("006a7780_TEST PASS\n");
    return 0;
}