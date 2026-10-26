// Behaviour test for 0x006A6F50 CThingFilter_HasPhysicsMesh::operator()
#include <cstdio>
class CPhysicsMesh { public: int x; };
class CTCPhysicsBase { public:
    CPhysicsMesh* m_mesh;
    virtual CPhysicsMesh* GetPhysicsMesh() { return m_mesh; }
};
class CThing { public:
    CTCPhysicsBase* PhysicsTC;
};
class CThingFilter_HasPhysicsMesh { public:
    bool operator()(CThing* thing) const
    {
        CTCPhysicsBase* physics = thing->PhysicsTC;
        if (physics)
            return physics->GetPhysicsMesh() != 0;
        return false;
    }
};
int main()
{
    CPhysicsMesh mesh; CTCPhysicsBase p; p.m_mesh = 0;
    CThing t; t.PhysicsTC = 0;
    CThingFilter_HasPhysicsMesh f;
    bool ok = !f(&t);
    t.PhysicsTC = &p; if (f(&t)) ok = false;
    p.m_mesh = &mesh; if (!f(&t)) ok = false;
    if (!ok) { std::printf("006a6f50_TEST FAIL\n"); return 1; }
    std::printf("006a6f50_TEST PASS\n");
    return 0;
}