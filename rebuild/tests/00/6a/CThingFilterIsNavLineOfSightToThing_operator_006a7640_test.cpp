// Behaviour test for 0x006A7640 CThingFilter_IsNavLineOfSightToThing::operator()
#include <cstdio>
struct C3DVector { float x, y, z; };
struct CThing {
    C3DVector m_pos;
    const C3DVector& GetPosition() const { return m_pos; }
};
struct CTCCreatureNavigation {
    float maxDist;
    const C3DVector* lastFrom;
    bool IsNavLineOfSight(const C3DVector* from, const C3DVector& to)
    {
        lastFrom = from;
        float d = to.x - from->x;
        return d < maxDist && -d < maxDist;
    }
};
struct CThingFilter_IsNavLineOfSightToThing {
    C3DVector*             Pos;
    CTCCreatureNavigation* PTCNav;
    bool operator()(CThing* thing) const { return PTCNav->IsNavLineOfSight(Pos, thing->GetPosition()); }
};
int main()
{
    C3DVector origin; origin.x = 0.0f; origin.y = 0.0f; origin.z = 0.0f;
    CTCCreatureNavigation nav; nav.maxDist = 10.0f; nav.lastFrom = 0;
    CThing t; t.m_pos.x = 4.0f; t.m_pos.y = 0.0f; t.m_pos.z = 0.0f;
    CThingFilter_IsNavLineOfSightToThing f; f.Pos = &origin; f.PTCNav = &nav;
    bool ok = f(&t) && nav.lastFrom == &origin;
    t.m_pos.x = 40.0f; if (f(&t)) ok = false;
    if (!ok) { std::printf("006a7640_TEST FAIL\n"); return 1; }
    std::printf("006a7640_TEST PASS\n");
    return 0;
}