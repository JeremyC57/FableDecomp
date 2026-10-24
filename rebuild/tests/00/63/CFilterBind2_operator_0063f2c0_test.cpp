#include <cstdio>
// Behaviour test: CFilter_Bind2<CThingFilter_OverlappingCircle, CWorldMap::CThingFilterBase>::operator() -- the thing's
// map-who position within the summed radii of the circle (2D, strictly inside), then the world-map filter.
struct C3DVector { float X, Y, Z; };
struct CTCPhysicsBase {
    C3DVector Pos; float Radius;
    C3DVector GetMapwhoPosition() const { return Pos; }
};
struct CThing { CTCPhysicsBase* PhysicsTC; int Tag; };
class CThingFilter_OverlappingCircle {
public:
    C3DVector SpherePos; float Radius;
    bool Overlaps(const C3DVector& pos, const CTCPhysicsBase* physics) const
    {
        float dx = pos.X - SpherePos.X;
        float dy = pos.Y - SpherePos.Y;
        float r = physics->Radius + Radius;
        float d2 = dx * dx;
        d2 += dy * dy;
        if (d2 < r * r)
            return true;
        return false;
    }
    bool operator()(const CThing& thing) const
    {
        const CTCPhysicsBase* physics = thing.PhysicsTC;
        if (physics)
            return Overlaps(physics->GetMapwhoPosition(), physics);
        return false;
    }
};
class CThingFilterBase {
public:
    int Calls;
    virtual bool operator()(const CThing& thing) const { ++const_cast<CThingFilterBase*>(this)->Calls; return thing.Tag != 0; }
    virtual ~CThingFilterBase() {}
};
template <class F1, class F2> class CFilter_Bind2 {
public:
    const F1* PF1; const F2* PF2;
    template <class T> bool operator()(const T& thing) const { return (*PF1)(thing) && (*PF2)(thing); }
};
int main()
{
    CThingFilter_OverlappingCircle circle;
    circle.SpherePos.X = 0.0f; circle.SpherePos.Y = 0.0f; circle.SpherePos.Z = 100.0f; circle.Radius = 2.0f;
    CThingFilterBase base; base.Calls = 0;
    CFilter_Bind2<CThingFilter_OverlappingCircle, CThingFilterBase> f = { &circle, &base };
    CTCPhysicsBase tc = { { 3.0f, 0.0f, -50.0f }, 1.5f };
    CThing t = { &tc, 1 };
    if (!f(t) || base.Calls != 1) { std::printf("0063f2c0_TEST FAIL\n"); return 1; }
    tc.Pos.X = 3.5f;                        // exactly on the summed radius: not inside
    if (f(t) || base.Calls != 1) { std::printf("0063f2c0_TEST FAIL\n"); return 1; }
    tc.Pos.X = 2.0f; tc.Pos.Y = 2.0f; t.Tag = 0;
    if (f(t) || base.Calls != 2) { std::printf("0063f2c0_TEST FAIL\n"); return 1; }
    CThing none = { 0, 1 };
    if (f(none) || base.Calls != 2) { std::printf("0063f2c0_TEST FAIL\n"); return 1; }
    std::printf("0063f2c0_TEST PASS\n");
    return 0;
}