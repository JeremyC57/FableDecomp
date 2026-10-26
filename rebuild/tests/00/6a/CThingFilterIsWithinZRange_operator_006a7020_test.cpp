// Behaviour test for 0x006A7020 CThingFilter_IsWithinZRange::operator()
#include <cstdio>
#include <math.h>
class C3DVector { public: float x, y, z; };
class CTCPhysicsBase { public: C3DVector m_position; };
class CThing { public: CTCPhysicsBase* PhysicsTC; };
class CThingFilter_IsWithinZRange { public:
    float Z;
    float Range;
    bool operator()(CThing* thing) const
    {
        CTCPhysicsBase* physics = thing->PhysicsTC;
        if (physics && fabs(physics->m_position.z - Z) < Range)
            return true;
        return false;
    }
};
int main()
{
    CTCPhysicsBase p; p.m_position.x = 0.0f; p.m_position.y = 0.0f; p.m_position.z = 7.0f;
    CThing t; t.PhysicsTC = 0;
    CThingFilter_IsWithinZRange f; f.Z = 10.0f; f.Range = 4.0f;
    bool ok = !f(&t);
    t.PhysicsTC = &p; if (!f(&t)) ok = false;       // |7-10| = 3 < 4
    f.Range = 3.0f; if (f(&t)) ok = false;          // strict
    p.m_position.z = 13.5f; f.Range = 4.0f; if (!f(&t)) ok = false;
    if (!ok) { std::printf("006a7020_TEST FAIL\n"); return 1; }
    std::printf("006a7020_TEST PASS\n");
    return 0;
}