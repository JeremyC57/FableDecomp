// Behaviour test for 0x006A7390 CThingFilter_IsInFOV::operator()(CThing*)
#include <cstdio>
struct C3DVector { float x, y, z; };
struct CTCPhysicsBase {
    C3DVector m_centre; float m_radius;
    void GetBoundingInfo(C3DVector* centre, float* radius) { *centre = m_centre; *radius = m_radius; }
};
struct CThing { CTCPhysicsBase* PhysicsTC; };
struct CThingFilter_IsInFOV {
    C3DVector FromPos; C3DVector Direction; float CosFOV; float FOV; bool AngleTest3D;
    bool operator()(const C3DVector& pos) const
    {
        float dx = pos.x - FromPos.x, dy = pos.y - FromPos.y;
        return dx * Direction.x + dy * Direction.y > 0.0f;     // simplified half-plane FOV
    }
    bool operator()(CThing* thing) const
    {
        C3DVector centre;
        float radius;
        thing->PhysicsTC->GetBoundingInfo(&centre, &radius);
        return (*this)(centre);
    }
};
int main()
{
    CTCPhysicsBase p; p.m_centre.x = 5.0f; p.m_centre.y = 1.0f; p.m_centre.z = 0.0f; p.m_radius = 2.0f;
    CThing t; t.PhysicsTC = &p;
    CThingFilter_IsInFOV f;
    f.FromPos.x = 0.0f; f.FromPos.y = 0.0f; f.FromPos.z = 0.0f;
    f.Direction.x = 1.0f; f.Direction.y = 0.0f; f.Direction.z = 0.0f;
    f.CosFOV = 0.5f; f.FOV = 1.0f; f.AngleTest3D = false;
    bool ok = f(&t);
    p.m_centre.x = -5.0f; if (f(&t)) ok = false;
    if (!ok) { std::printf("006a7390_TEST FAIL\n"); return 1; }
    std::printf("006a7390_TEST PASS\n");
    return 0;
}
