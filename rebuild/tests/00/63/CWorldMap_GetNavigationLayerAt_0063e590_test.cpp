// Behaviour test for CWorldMap::GetNavigationLayerAt (0x0063E590): per-map navigation path (flag on, no ignore thing, engine
// query false) answers from the loaded map; otherwise the rough-area search over navigation-bearing things round the position.
#include <cstdio>
#include <cmath>
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
class C2DBoxI { public:
    long x0, y0, x1, y1;
    C2DBoxI(long ax0, long ay0, long ax1, long ay1) : x0(ax0), y0(ay0), x1(ax1), y1(ay1) { }
};
class CThing { public:                                      // test model: a physical thing with a navigation layer
    C3DVector Pos;
    float     Radius;
    bool      HasNavigation;
    long      Layer;
};
class CThingPhysical;
static CThing* g_things[8];
static int     g_thingCount = 0;
static C2DBoxI g_lastBox(0, 0, 0, 0);
class CThingFilter_HasNavigationInformation { public:
    CThingFilter_HasNavigationInformation() { }
    bool operator()(const CThing& t) const { return t.HasNavigation; }
};
class CThingFilter_IsNot { public:
    const CThing* Thing;
    CThingFilter_IsNot(const CThing* thing) : Thing(thing) { }
    bool operator()(const CThing& t) const { return &t != Thing; }
};
class CThingFilter_OverlappingBoundingBox { public:
    C3DVector Pos;
    float     Radius;
    CThingFilter_OverlappingBoundingBox(const C3DVector& pos, float radius) : Pos(pos), Radius(radius) { }
    bool operator()(const CThing& t) const
    {
        return std::fabs(t.Pos.X - Pos.X) <= t.Radius + Radius && std::fabs(t.Pos.Y - Pos.Y) <= t.Radius + Radius;
    }
};
template <class F1, class F2> class CFilter_Bind2 { public:
    const F1* P1; const F2* P2;
    CFilter_Bind2(const F1& f1, const F2& f2) : P1(&f1), P2(&f2) { }
    bool operator()(const CThing& t) const { return (*P1)(t) && (*P2)(t); }
};
template <class F1, class F2, class F3> class CFilter_Bind3 { public:
    const F1* P1; const F2* P2; const F3* P3;
    CFilter_Bind3(const F1& f1, const F2& f2, const F3& f3) : P1(&f1), P2(&f2), P3(&f3) { }
    bool operator()(const CThing& t) const { return (*P1)(t) && (*P2)(t) && (*P3)(t); }
};
class CThingFindNavigationLayerAt { public:
    C3DVector Pos;
    long      Layer;
    CThingFindNavigationLayerAt(const C3DVector& pos) : Pos(pos), Layer(0) { }
    float GetLayer() const { return (float)Layer; }
    bool operator()(CThing& t) { Layer = t.Layer; return true; }
};
typedef CFilter_Bind2<CThingFilter_HasNavigationInformation, CThingFilter_OverlappingBoundingBox> CNavFilter2;
typedef CFilter_Bind3<CThingFilter_IsNot, CThingFilter_HasNavigationInformation, CThingFilter_OverlappingBoundingBox> CNavFilter3;
class CThingSearchTools { public:
    template <class T, class F, class P> bool ForEachInRoughArea(const C2DBoxI& box, const F& filter, P& proc)
    {
        g_lastBox = box;
        for (int i = 0; i < g_thingCount; ++i)
            if (filter(*g_things[i]) && !proc(*g_things[i]))
                return false;
        return true;
    }
};
class CWorld { public:
    unsigned char      _pad00[0x20];
    CThingSearchTools* ThingSearchTools;
};
class CEngine { public:
    virtual bool IsLevelPersistent() { return Persistent; }
    bool Persistent;
};
static CEngine g_engine;
CEngine* g_pEngine = &g_engine;
bool g_bMapNavigationLayers = false;
class CMap {
public:
    long NavLayer;
    long GetNavigationLayerAt(const C3DVector& pos) const { return NavLayer; }
};
class CWorldMap {
public:
    virtual bool IsMapLoaded(long map) const { return map == LoadedMap; }
    virtual long GetMapNumberAt(const C3DVector& pos) const { return pos.X >= 0.0f ? 1 : 0; }
    virtual const CMap& PeekMap(long map) const { return Map; }
    long GetNavigationLayerAt(const C3DVector& pos, const CThing* ignore) const;
    CWorld* World;
    long    LoadedMap;
    CMap    Map;
};
long CWorldMap::GetNavigationLayerAt(const C3DVector& pos, const CThing* ignore) const
{
    if (g_bMapNavigationLayers && ignore == 0 && !g_pEngine->IsLevelPersistent()) {
        long map = GetMapNumberAt(pos);
        if (map != 0 && IsMapLoaded(map))
        {
            const CMap& theMap = PeekMap(map);
            return theMap.GetNavigationLayerAt(pos);
        }
        return 0;
    }
    long x = (long)pos.X;
    long y = (long)pos.Y;
    C2DBoxI box(x - 1, y - 1, x + 1, y + 1);
    CThingFindNavigationLayerAt finder(pos);
    if (ignore == 0) {
        CThingSearchTools* tools = World->ThingSearchTools;
        tools->ForEachInRoughArea<CThingPhysical>(box, CNavFilter2(CThingFilter_HasNavigationInformation(), CThingFilter_OverlappingBoundingBox(pos, 0.1f)), finder);
    }
    else {
        CThingFilter_IsNot isNot(ignore);
        CThingSearchTools* tools = World->ThingSearchTools;
        tools->ForEachInRoughArea<CThingPhysical>(box, CNavFilter3(isNot, CThingFilter_HasNavigationInformation(), CThingFilter_OverlappingBoundingBox(pos, 0.1f)), finder);
    }
    return finder.GetLayer();
}
int main()
{
    int fails = 0;
    CThingSearchTools tools;
    CWorld world; world.ThingSearchTools = &tools;
    CWorldMap wm; wm.World = &world; wm.LoadedMap = 1; wm.Map.NavLayer = 7;
    CThing a = { C3DVector(10.0f, 10.0f, 0.0f), 1.0f, true, 3 };
    CThing b = { C3DVector(10.5f, 10.0f, 0.0f), 1.0f, true, 5 };
    CThing c = { C3DVector(10.0f, 10.5f, 0.0f), 1.0f, false, 9 };    // no navigation information: never counted
    CThing d = { C3DVector(40.0f, 40.0f, 0.0f), 1.0f, true, 11 };    // out of range
    g_things[0] = &a; g_things[1] = &b; g_things[2] = &c; g_things[3] = &d; g_thingCount = 4;
    C3DVector pos(10.2f, 10.1f, 0.0f);
    // search path (flag off): the last matching thing sets the layer, the box is 2x2 round the truncated position
    if (wm.GetNavigationLayerAt(pos, 0) != 5) { std::printf("case1 fail\n"); ++fails; }
    if (g_lastBox.x0 != 9 || g_lastBox.y0 != 9 || g_lastBox.x1 != 11 || g_lastBox.y1 != 11) { std::printf("box fail\n"); ++fails; }
    // ignoring b leaves a
    if (wm.GetNavigationLayerAt(pos, &b) != 3) { std::printf("case2 fail\n"); ++fails; }
    // nothing in range -> 0
    if (wm.GetNavigationLayerAt(C3DVector(-20.0f, -20.0f, 0.0f), 0) != 0) { std::printf("case3 fail\n"); ++fails; }
    // per-map path
    g_bMapNavigationLayers = true; g_engine.Persistent = false;
    if (wm.GetNavigationLayerAt(pos, 0) != 7) { std::printf("case4 fail\n"); ++fails; }
    if (wm.GetNavigationLayerAt(C3DVector(-1.0f, 0.0f, 0.0f), 0) != 0) { std::printf("case5 fail\n"); ++fails; }  // map 0
    wm.LoadedMap = 2;
    if (wm.GetNavigationLayerAt(pos, 0) != 0) { std::printf("case6 fail\n"); ++fails; }                             // not loaded
    // an ignore thing or a persistent level falls back to the search
    if (wm.GetNavigationLayerAt(pos, &a) != 5) { std::printf("case7 fail\n"); ++fails; }
    g_engine.Persistent = true; wm.LoadedMap = 1;
    if (wm.GetNavigationLayerAt(pos, 0) != 5) { std::printf("case8 fail\n"); ++fails; }
    if (fails) { std::printf("0063e590_TEST FAIL\n"); return 1; }
    std::printf("0063e590_TEST PASS\n");
    return 0;
}