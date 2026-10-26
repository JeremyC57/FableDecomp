// Behaviour test for 0x00727500 for_each(CThingCreatureBase**, CThingCreatureBase**, CApplyAdditionalImpulseIfInCombatAndTooClose)
// (the functor's operator() modelled as a counter on the creature).
#include <cstdio>
class CThingCreatureBase { public: int pushes; };
class CTCPhysicsBase { public: int id; };
class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};
class CApplyAdditionalImpulseIfInCombatAndTooClose { public:
    C3DVector Position;
    float Radius;
    CTCPhysicsBase* Owner;
    CThingCreatureBase* OwnerCreature;
    CApplyAdditionalImpulseIfInCombatAndTooClose(const C3DVector& position, float radius, CTCPhysicsBase* owner, CThingCreatureBase* ownerCreature)
        : Position(position), Radius(radius), Owner(owner), OwnerCreature(ownerCreature) {}
    void operator()(CThingCreatureBase* other) { if (other != OwnerCreature) other->pushes += 1; Position.X += 1.0f; }
};
static CApplyAdditionalImpulseIfInCombatAndTooClose for_each_(CThingCreatureBase** first, CThingCreatureBase** last, CApplyAdditionalImpulseIfInCombatAndTooClose f)
{
    for (; first != last; ++first)
        f(*first);
    return f;
}
int main()
{
    CThingCreatureBase c[4];
    CThingCreatureBase* p[4];
    for (int i = 0; i < 4; ++i) { c[i].pushes = 0; p[i] = &c[i]; }
    CTCPhysicsBase owner; owner.id = 1;
    CApplyAdditionalImpulseIfInCombatAndTooClose f(C3DVector(0.0f, 1.0f, 2.0f), 3.0f, &owner, &c[2]);
    CApplyAdditionalImpulseIfInCombatAndTooClose r = for_each_(p, p + 4, f);
    if (c[0].pushes != 1 || c[1].pushes != 1 || c[2].pushes != 0 || c[3].pushes != 1) { std::printf("00727500_TEST FAIL\n"); return 1; }
    if (r.Position.X != 4.0f || r.Radius != 3.0f || r.Owner != &owner || r.OwnerCreature != &c[2] || f.Position.X != 0.0f) { std::printf("00727500_TEST FAIL\n"); return 1; }
    std::printf("00727500_TEST PASS\n");
    return 0;
}