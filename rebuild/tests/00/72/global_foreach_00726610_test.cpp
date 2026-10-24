// Behaviour test for 0x00726610 for_each(CThingCreatureBase**, CThingCreatureBase**, CApplyAdditionalPlayerImpulseIfTooClose)
// (the functor's operator() modelled as a counter on the creature).
#include <cstdio>
class CThingCreatureBase { public: int pushes; };
class CTCPhysicsBase { public: int id; };
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};
struct CApplyAdditionalPlayerImpulseIfTooClose {
    C3DVector Position;
    float Radius;
    CTCPhysicsBase* Owner;
    CApplyAdditionalPlayerImpulseIfTooClose(const C3DVector& position, float radius, CTCPhysicsBase* owner)
        : Position(position), Radius(radius), Owner(owner) {}
    void operator()(CThingCreatureBase* other) { other->pushes += (int)Radius; Radius += 1.0f; }
};
static CApplyAdditionalPlayerImpulseIfTooClose for_each_(CThingCreatureBase** first, CThingCreatureBase** last, CApplyAdditionalPlayerImpulseIfTooClose f)
{
    for (; first != last; ++first)
        f(*first);
    return f;
}
int main()
{
    CThingCreatureBase c[3];
    CThingCreatureBase* p[3];
    for (int i = 0; i < 3; ++i) { c[i].pushes = 0; p[i] = &c[i]; }
    CTCPhysicsBase owner; owner.id = 5;
    CApplyAdditionalPlayerImpulseIfTooClose f(C3DVector(1.0f, 2.0f, 3.0f), 2.0f, &owner);
    CApplyAdditionalPlayerImpulseIfTooClose r = for_each_(p, p + 3, f);
    if (c[0].pushes != 2 || c[1].pushes != 3 || c[2].pushes != 4) { std::printf("00726610_TEST FAIL\n"); return 1; }
    if (r.Radius != 5.0f || r.Owner != &owner || r.Position.Y != 2.0f || f.Radius != 2.0f) { std::printf("00726610_TEST FAIL\n"); return 1; }
    CApplyAdditionalPlayerImpulseIfTooClose e = for_each_(p, p, f);
    if (e.Radius != 2.0f || e.Position.Z != 3.0f) { std::printf("00726610_TEST FAIL\n"); return 1; }
    std::printf("00726610_TEST PASS\n");
    return 0;
}