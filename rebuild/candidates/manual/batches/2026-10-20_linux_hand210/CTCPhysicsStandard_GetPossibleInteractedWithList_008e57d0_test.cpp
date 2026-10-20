// Behaviour test for CTCPhysicsStandard::GetPossibleInteractedWithList (0x008E57D0): no search unless the body interacts or
// collides with things; the search gets the caller's box and list and a Bind3 of {this, exclude-creatures}, the sphere at the
// position with radius + extra, and HasTC(2).
#include <cstdio>
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
struct C2DBoxI { long TLX, TLY, BRX, BRY; };
struct CThing;
template <class T> struct vector { T* _M_start; T* _M_finish; T* _M_end_of_storage; };
struct CTCPhysicsBase;
struct CThingFilter_CanCollideOrInteractWithThing {
    CTCPhysicsBase* TCPhysics;
    bool            ExcludeCreatures;
    CThingFilter_CanCollideOrInteractWithThing(CTCPhysicsBase* physics, bool excludeCreatures)
        : TCPhysics(physics), ExcludeCreatures(excludeCreatures) { }
};
struct CThingFilter_OverlappingSphere {
    C3DVector SpherePos;
    float     Radius;
    CThingFilter_OverlappingSphere(const C3DVector& pos, float radius) : SpherePos(pos), Radius(radius) { }
};
struct CThingFilter_HasTC {
    long Type;
    CThingFilter_HasTC(long type) : Type(type) { }
};
template <class A, class B, class C> struct CFilter_Bind3 {
    const A* F1;
    const B* F2;
    const C* F3;
    CFilter_Bind3(const A& a, const B& b, const C& c) : F1(&a), F2(&b), F3(&c) { }
};
static int g_searches = 0; static const C2DBoxI* g_box = 0; static void* g_out = 0;
static CTCPhysicsBase* g_physics = 0; static bool g_exclude = false; static float g_sx = 0, g_sy = 0, g_sz = 0, g_sr = 0; static long g_type = 0;
struct CThingSearchTools {
    template <class T, class TContainer, class TFilter>
    void GetAllThingsInRoughArea(const C2DBoxI& box, const TFilter& filter, TContainer& out) const
    {
        ++g_searches; g_box = &box; g_out = &out;
        g_physics = filter.F1->TCPhysics; g_exclude = filter.F1->ExcludeCreatures;
        g_sx = filter.F2->SpherePos.X; g_sy = filter.F2->SpherePos.Y; g_sz = filter.F2->SpherePos.Z; g_sr = filter.F2->Radius;
        g_type = filter.F3->Type;
    }
};
static CThingSearchTools g_tools;
struct CTCPhysicsBase {
    bool m_collides, m_oneWay, m_interacts, m_exclude;
    C3DVector Position;
    float     Radius;
    CTCPhysicsBase() : Position(0, 0, 0) { }
    bool GetCollidesWithThings() const { return m_collides; }
    bool GetCollidesWithThingsOneWay() const { return m_oneWay; }
    bool GetInteractsWithThings() const { return m_interacts; }
    bool IsExcludingCreatures() const { return m_exclude; }
    const CThingSearchTools* PeekThingSearchTools() const { return &g_tools; }
};
struct CTCPhysicsStandard : CTCPhysicsBase {
    void GetPossibleInteractedWithList(const C2DBoxI& box, vector<CThing*>& things, float extraRadius) const;
};

void CTCPhysicsStandard::GetPossibleInteractedWithList(const C2DBoxI& box, vector<CThing*>& things, float extraRadius) const
{
    if (GetInteractsWithThings() || GetCollidesWithThings() || GetCollidesWithThingsOneWay()) {
        CThingFilter_CanCollideOrInteractWithThing canFilter(const_cast<CTCPhysicsStandard*>(this), IsExcludingCreatures());
        CThingFilter_OverlappingSphere sphereFilter(Position, extraRadius + Radius);
        CThingFilter_HasTC physicsFilter(2);
        PeekThingSearchTools()->GetAllThingsInRoughArea<const CThing>(box,
            CFilter_Bind3<CThingFilter_CanCollideOrInteractWithThing, CThingFilter_OverlappingSphere, CThingFilter_HasTC>(
                canFilter, sphereFilter, physicsFilter), things);
    }
}

int main()
{
    int bad = 0;
    CTCPhysicsStandard p; p.m_collides = p.m_oneWay = p.m_interacts = false; p.m_exclude = true;
    p.Position = C3DVector(1, 2, 3); p.Radius = 0.5f;
    C2DBoxI box = { 0, 0, 10, 10 }; vector<CThing*> things = { 0, 0, 0 };
    p.GetPossibleInteractedWithList(box, things, 2.0f);
    if (g_searches != 0) bad |= 1;
    p.m_oneWay = true;
    p.GetPossibleInteractedWithList(box, things, 2.0f);
    if (g_searches != 1 || g_box != &box || g_out != &things || g_physics != &p || !g_exclude) bad |= 2;
    if (g_sx != 1 || g_sy != 2 || g_sz != 3 || g_sr != 2.5f || g_type != 2) bad |= 4;
    p.m_oneWay = false; p.m_collides = true; p.m_exclude = false;
    p.GetPossibleInteractedWithList(box, things, 0.0f);
    if (g_searches != 2 || g_exclude || g_sr != 0.5f) bad |= 8;
    p.m_collides = false; p.m_interacts = true;
    p.GetPossibleInteractedWithList(box, things, 1.0f);
    if (g_searches != 3) bad |= 16;
    if (bad) { std::printf("008e57d0_TEST FAIL\n"); return 1; }
    std::printf("008e57d0_TEST PASS\n");
    return 0;
}
