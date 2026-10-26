// Behaviour test for 0x007249C0 CIsCreatureIsIntersecting::operator() (callees modelled).
#include <cstdio>
class C3DVector { public: float X, Y, Z; };
class CTCPhysicsBase { public: C3DVector Position; float Radius; };
class CThing {
public:
    unsigned char m_type;
    CTCPhysicsBase* PhysicsTC;
    unsigned long ThingTypeFlags;
    bool busy, blocking;
    const C3DVector& GetPosition() const { return PhysicsTC->Position; }
};
class CThingCreatureBase : public CThing {
public:
    bool IsPerformingAnyAction() const { return busy; }
    bool IsBlocking() const { return blocking; }
};
class CIsCreatureIsIntersecting { public:
    CThing* Owner;
    bool operator()(CThing* thing) const
    {
        bool result = false;
        CTCPhysicsBase* phys = thing->PhysicsTC;
        if (thing->ThingTypeFlags & 8) {
            const C3DVector& pos = Owner->GetPosition();
            float dx = pos.X - phys->Position.X;
            float dy = pos.Y - phys->Position.Y;
            float dz = pos.Z - phys->Position.Z;
            float r = Owner->PhysicsTC->Radius + phys->Radius;
            if (dx * dx + dy * dy + dz * dz < r * r) {
                result = true;
                if (thing->m_type == 2 && ((CThingCreatureBase*)Owner)->IsPerformingAnyAction() && ((CThingCreatureBase*)thing)->IsBlocking())
                    return false;
            }
        }
        return result;
    }
};
static void setup(CThingCreatureBase& t, CTCPhysicsBase& p, float x, float y, float z, float r, unsigned char type, unsigned long flags)
{
    p.Position.X = x; p.Position.Y = y; p.Position.Z = z; p.Radius = r;
    t.m_type = type; t.PhysicsTC = &p; t.ThingTypeFlags = flags; t.busy = false; t.blocking = false;
}
int main()
{
    CThingCreatureBase owner, near1, far1, flagless, blocker;
    CTCPhysicsBase po, pn, pf, pl, pb;
    setup(owner, po, 0, 0, 0, 1.0f, 1, 8);
    setup(near1, pn, 1.5f, 0, 0, 1.0f, 3, 8);
    setup(far1, pf, 0, 3.0f, 0, 0.5f, 3, 8);
    setup(flagless, pl, 0, 0, 0.5f, 1.0f, 3, 0);
    setup(blocker, pb, 0, 0, 1.0f, 1.0f, 2, 8 | 1);
    CIsCreatureIsIntersecting pred;
    pred.Owner = &owner;
    if (!pred(&near1) || pred(&far1) || pred(&flagless)) { std::printf("007249c0_TEST FAIL\n"); return 1; }
    if (!pred(&blocker)) { std::printf("007249c0_TEST FAIL\n"); return 1; }
    owner.busy = true;
    if (!pred(&blocker)) { std::printf("007249c0_TEST FAIL\n"); return 1; }
    blocker.blocking = true;
    if (pred(&blocker) || !pred(&near1)) { std::printf("007249c0_TEST FAIL\n"); return 1; }
    std::printf("007249c0_TEST PASS\n");
    return 0;
}