// Behaviour test for 0x006A76B0 CThingFilter_In2DRange::operator()
#include <cstdio>
struct C3DVector { float x, y, z; };
struct CTCPhysicsBase { C3DVector m_position; };
struct CThing { CTCPhysicsBase* PhysicsTC; };
struct CThingFilter_In2DRange {
    C3DVector Pos;
    float     Range;
    bool operator()(CThing* thing) const
    {
        CTCPhysicsBase* physics = thing->PhysicsTC;
        if (physics) {
            float range = Range;
            float dx = Pos.x - physics->m_position.x;
            float dy = Pos.y - physics->m_position.y;
            return dx * dx + dy * dy < range * range;
        }
        return false;
    }
};
int main()
{
    CTCPhysicsBase p; p.m_position.x = 3.0f; p.m_position.y = 4.0f; p.m_position.z = 100.0f;
    CThing t; t.PhysicsTC = 0;
    CThingFilter_In2DRange f; f.Pos.x = 0.0f; f.Pos.y = 0.0f; f.Pos.z = 0.0f; f.Range = 5.5f;
    bool ok = !f(&t);
    t.PhysicsTC = &p;
    if (!f(&t)) ok = false;                         // 2D distance 5 < 5.5, z ignored
    f.Range = 5.0f; if (f(&t)) ok = false;          // strict
    if (!ok) { std::printf("006a76b0_TEST FAIL\n"); return 1; }
    std::printf("006a76b0_TEST PASS\n");
    return 0;
}
