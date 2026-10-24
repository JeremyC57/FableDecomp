// Behaviour test for 0x00669040 CFilter_Bind3<SolidToThing, PhysicsFlagsSolidToThing, OverlappingCircle>::operator() (model copy)
#include <cstdio>
struct Vec3 { float X, Y, Z; };
struct Physics {
    bool collides, oneWay; float Radius; Vec3 pos;
    bool GetCollidesWithThings() const { return collides; }
    bool GetCollidesWithThingsOneWay() const { return oneWay; }
    Vec3 GetMapwhoPosition() const { return pos; }
};
struct Thing { Physics* PhysicsTC; };
struct SolidToThing { const Thing* SourceThing; void* PTC; bool operator()(const Thing& t) const { return &t != SourceThing; } };
struct PhysicsFlags {
    const Thing* SourceThing; Physics* SourceThingPhysicsTC;
    bool operator()(const Thing& t) const
    {
        Physics* p = t.PhysicsTC;
        if (!SourceThingPhysicsTC || !p) return false;
        if (!(SourceThingPhysicsTC->GetCollidesWithThings() || SourceThingPhysicsTC->GetCollidesWithThingsOneWay())) return false;
        return p->GetCollidesWithThings();
    }
};
struct Circle {
    Vec3 SpherePos; float Radius;
    bool operator()(const Thing& t) const
    {
        Physics* p = t.PhysicsTC;
        if (!p) return false;
        Vec3 pos = p->GetMapwhoPosition();
        float dx = pos.X - SpherePos.X, dy = pos.Y - SpherePos.Y, r = p->Radius + Radius;
        float d2 = dx * dx; d2 += dy * dy;
        return d2 < r * r;
    }
};
struct Bind3 { const SolidToThing* F1; const PhysicsFlags* F2; const Circle* F3;
    bool operator()(const Thing& t) const { return (*F1)(t) && (*F2)(t) && (*F3)(t); } };
int main()
{
    Physics srcP = { false, true, 1.0f, { 0, 0, 0 } };
    Thing src = { &srcP };
    Physics p = { true, false, 1.0f, { 3.0f, 0.0f, 100.0f } };
    Thing t = { &p };
    SolidToThing f1 = { &src, 0 };
    PhysicsFlags f2 = { &src, &srcP };
    Circle f3 = { { 0, 0, 0 }, 2.5f };            // r = 3.5 > 3 (Z ignored)
    Bind3 b = { &f1, &f2, &f3 };
    bool ok = b(t);
    ok = ok && !b(src);                           // the source thing itself
    f3.Radius = 1.5f; ok = ok && !b(t);           // r = 2.5 < 3
    f3.Radius = 2.5f; p.collides = false; ok = ok && !b(t);
    p.collides = true; srcP.oneWay = false; ok = ok && !b(t);
    srcP.collides = true; ok = ok && b(t);
    Thing none = { 0 }; ok = ok && !b(none);
    std::printf(ok ? "00669040_TEST PASS\n" : "00669040_TEST FAIL\n");
    return ok ? 0 : 1;
}