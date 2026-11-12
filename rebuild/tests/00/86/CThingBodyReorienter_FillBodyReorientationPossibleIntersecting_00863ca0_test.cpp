// Behaviour test for CThingBodyReorienter::FillBodyReorientationPossibleIntersecting (0x00863CA0): a copy of the model with
// small stand-ins for the engine (a thing list for the mapwho search, a plain array for the intelligent-pointer vector).
// Checks the square's size (diameter, clamped below at 1, truncated like the position), the physics-mesh filter, the
// append to PossibleIntersectingThings and the three flags in the "nothing found" case.
#include <cstdio>

class C3DVector { public: float X, Y, Z; };
class C2DCoordI { public:
    long X, Y;
    C2DCoordI(long x, long y) : X(x), Y(y) { }
};
class C2DBoxI { public:
    long TLX, TLY, BRX, BRY;
    C2DBoxI(const C2DCoordI& p, long radius) : TLX(p.X - radius), TLY(p.Y - radius), BRX(p.X + radius), BRY(p.Y + radius) { }
};
class CThing { public:
    C3DVector Pos;
    float     Radius;
    bool      HasPhysicsMesh;
    const C3DVector& GetPos() const { return Pos; }
    float GetDiameter() const { return Radius * 2.0f; }
};
class CThingFilter_HasPhysicsMesh { public:
    CThingFilter_HasPhysicsMesh() { }
    bool operator()(const CThing& thing) const { return thing.HasPhysicsMesh; }
};
struct CThingList {
    CThing* Items[16];
    int     Count;
    CThingList() : Count(0) { }
    void push_back(CThing* thing) { Items[Count++] = thing; }
    bool empty() const { return Count == 0; }
};
class CThingSearchTools { public:
    CThingList World;
    template <class TFilter>
    void GetAllThingsInRoughArea(const C2DBoxI& area, const TFilter& filter, CThingList& result) const
    {
        for (int i = 0; i < World.Count; ++i) {
            CThing* thing = World.Items[i];
            long x = (long)thing->Pos.X;
            long y = (long)thing->Pos.Y;
            if (x >= area.TLX && x <= area.BRX && y >= area.TLY && y <= area.BRY && filter(*thing))
                result.push_back(thing);
        }
    }
};
static CThingSearchTools g_SearchTools;

class CThingBodyReorienter { public:
    bool       BodyReorientationDone;
    bool       StillCheckingForNewBodyReorientationAngle;
    bool       HasPossibleIntersectingThings;
    CThingList PossibleIntersectingThings;
    CThing*    Body;
    CThingBodyReorienter(CThing* body)
        : BodyReorientationDone(false), StillCheckingForNewBodyReorientationAngle(false), HasPossibleIntersectingThings(false),
          Body(body) { }
    CThingSearchTools& GetThingSearchTools() const { return g_SearchTools; }
    void FillBodyReorientationPossibleIntersecting();
};

void CThingBodyReorienter::FillBodyReorientationPossibleIntersecting()
{
    HasPossibleIntersectingThings = true;
    StillCheckingForNewBodyReorientationAngle = true;
    float size = Body->GetDiameter();
    if (size < 1.0f)
        size = 1.0f;
    const C3DVector& pos = Body->GetPos();
    C2DBoxI box(C2DCoordI((long)pos.X, (long)pos.Y), (long)size);
    CThingList things;
    CThingFilter_HasPhysicsMesh filter;
    GetThingSearchTools().GetAllThingsInRoughArea(box, filter, things);
    for (int i = 0; i < things.Count; ++i)
        PossibleIntersectingThings.push_back(things.Items[i]);
    if (PossibleIntersectingThings.empty()) {
        BodyReorientationDone = true;
        StillCheckingForNewBodyReorientationAngle = false;
        HasPossibleIntersectingThings = false;
    }
}

static CThing MakeThing(float x, float y, float radius, bool mesh)
{
    CThing thing;
    thing.Pos.X = x; thing.Pos.Y = y; thing.Pos.Z = 0.0f;
    thing.Radius = radius;
    thing.HasPhysicsMesh = mesh;
    return thing;
}

int main()
{
    bool ok = true;
    CThing inside   = MakeThing(12.0f, 22.0f, 1.0f, true);     // inside the 7..13 x 17..23 square
    CThing outside  = MakeThing(14.0f, 20.0f, 1.0f, true);     // one past the right edge
    CThing noMesh   = MakeThing(8.0f, 18.0f, 1.0f, false);     // inside, but filtered out
    CThing onCorner = MakeThing(7.9f, 23.9f, 1.0f, true);      // truncates onto the corner (7, 23)
    g_SearchTools.World.push_back(&inside);
    g_SearchTools.World.push_back(&outside);
    g_SearchTools.World.push_back(&noMesh);
    g_SearchTools.World.push_back(&onCorner);

    // 1. radius 1.5 at (10.7, 20.2): size 3, centre (10, 20), box 7..13 x 17..23.
    CThing body = MakeThing(10.7f, 20.2f, 1.5f, false);
    CThingBodyReorienter found(&body);
    found.FillBodyReorientationPossibleIntersecting();
    if (found.PossibleIntersectingThings.Count != 2 || found.PossibleIntersectingThings.Items[0] != &inside ||
        found.PossibleIntersectingThings.Items[1] != &onCorner)
        ok = false;
    if (!found.HasPossibleIntersectingThings || !found.StillCheckingForNewBodyReorientationAngle || found.BodyReorientationDone)
        ok = false;

    // 2. a tiny body: the size clamps to 1 (unclamped, (long)0.4 == 0 would only cover the centre cell).
    CThing tiny = MakeThing(11.2f, 21.6f, 0.2f, false);        // centre (11, 21): box 10..12 x 20..22 holds `inside` only
    CThingBodyReorienter clamped(&tiny);
    clamped.FillBodyReorientationPossibleIntersecting();
    if (clamped.PossibleIntersectingThings.Count != 1 || clamped.PossibleIntersectingThings.Items[0] != &inside)
        ok = false;
    if (!clamped.HasPossibleIntersectingThings || clamped.BodyReorientationDone)
        ok = false;

    // 3. nothing with a physics mesh near the body: done at once, both flags cleared again.
    CThing lonely = MakeThing(100.0f, 100.0f, 2.0f, false);
    CThingBodyReorienter none(&lonely);
    none.FillBodyReorientationPossibleIntersecting();
    if (none.PossibleIntersectingThings.Count != 0 || !none.BodyReorientationDone ||
        none.StillCheckingForNewBodyReorientationAngle || none.HasPossibleIntersectingThings)
        ok = false;

    // 4. the found things are appended: an earlier entry keeps the check alive even when this search finds nothing.
    CThingBodyReorienter kept(&lonely);
    kept.PossibleIntersectingThings.push_back(&outside);
    kept.FillBodyReorientationPossibleIntersecting();
    if (kept.PossibleIntersectingThings.Count != 1 || kept.BodyReorientationDone ||
        !kept.StillCheckingForNewBodyReorientationAngle || !kept.HasPossibleIntersectingThings)
        ok = false;

    if (!ok) { std::printf("00863ca0_TEST FAIL\n"); return 1; }
    std::printf("00863ca0_TEST PASS\n");
    return 0;
}