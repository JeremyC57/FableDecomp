// Behaviour test for 0x008A8010 CGameScriptInterface::SmashAllWindowsWithinRadiusOfPoint (C++03). The model is the row's: the
// point's X / Y and the radius are truncated toward zero (x, y, r), the rough-area search box is (x - r, y - r)..(x + r, y + r),
// the search returns the things in the box that hold a smashable component (TCI_SMASHABLE = 78, the filter), and each one
// found has its component's Smash called once (GetTC then the virtual call). The search here is a stand-in that tests
// containment with TL inclusive and BR exclusive, as the cell walk of the rough-area body does.
#include <cstdio>
#include <vector>

enum { TCI_SMASHABLE = 78 };
struct C3DVector { float X, Y, Z; C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { } };
struct C2DCoordI { long X, Y; C2DCoordI(long x, long y) : X(x), Y(y) { } };
struct C2DBoxI {
    long TLX, TLY, BRX, BRY;
    C2DBoxI(const C2DCoordI& p, long radius) { long x = p.X; long y = p.Y; TLX = x - radius; TLY = y - radius; BRX = x + radius; BRY = y + radius; }
};
struct CTCSmashableWindow {
    int SmashCount;
    CTCSmashableWindow() : SmashCount(0) { }
    virtual ~CTCSmashableWindow() { }
    virtual void Smash() { ++SmashCount; }
};
struct CThing {
    long CellX, CellY;
    bool HasSmashable;
    CTCSmashableWindow Window;
    CThing(long x, long y, bool smashable) : CellX(x), CellY(y), HasSmashable(smashable) { }
    bool HasTC(int type) const { return type == TCI_SMASHABLE && HasSmashable; }
    bool GetTC(CTCSmashableWindow** out) { if (!HasTC(TCI_SMASHABLE)) return false; *out = &Window; return true; }
};
struct CThingFilter_HasTC {
    int Type;
    CThingFilter_HasTC(int type) : Type(type) { }
    bool operator()(const CThing& thing) const { return thing.HasTC(Type); }
};
static C2DBoxI g_LastBox(C2DCoordI(0, 0), 0);
static void GetAllThingsInRoughArea(std::vector<CThing*>& world, const C2DBoxI& area, const CThingFilter_HasTC& filter,
                                    std::vector<CThing*>& result)
{
    g_LastBox = area;
    for (size_t i = 0; i < world.size(); ++i) {
        CThing* t = world[i];
        if (t->CellX >= area.TLX && t->CellX < area.BRX && t->CellY >= area.TLY && t->CellY < area.BRY && filter(*t))
            result.push_back(t);
    }
}
static void SmashAllWindowsWithinRadiusOfPoint(std::vector<CThing*>& world, const C3DVector& point, float radius)
{
    long x = (long)point.X;
    long y = (long)point.Y;
    C2DBoxI area(C2DCoordI(x, y), (long)radius);
    std::vector<CThing*> things;
    GetAllThingsInRoughArea(world, area, CThingFilter_HasTC(TCI_SMASHABLE), things);
    for (std::vector<CThing*>::iterator it = things.begin(); it != things.end(); ++it) {
        CTCSmashableWindow* window;
        if ((*it)->GetTC(&window))
            window->Smash();
    }
}
int main()
{
    int fails = 0;
    CThing a(10, 10, true), b(14, 6, true), c(15, 10, true), d(12, 12, false), e(5, 5, true), f(-3, -2, true);
    std::vector<CThing*> world;
    world.push_back(&a); world.push_back(&b); world.push_back(&c); world.push_back(&d); world.push_back(&e); world.push_back(&f);
    SmashAllWindowsWithinRadiusOfPoint(world, C3DVector(10.9f, 10.2f, 99.0f), 4.8f);   // x 10, y 10, r 4: box (6,6)..(14,14)
    if (g_LastBox.TLX != 6 || g_LastBox.TLY != 6 || g_LastBox.BRX != 14 || g_LastBox.BRY != 14) ++fails;
    if (a.Window.SmashCount != 1) ++fails;     // inside
    if (b.Window.SmashCount != 0) ++fails;     // x 14 = BR (exclusive)
    if (c.Window.SmashCount != 0) ++fails;     // outside
    if (d.Window.SmashCount != 0) ++fails;     // inside but no smashable component
    if (e.Window.SmashCount != 0) ++fails;     // x 5 < TL
    SmashAllWindowsWithinRadiusOfPoint(world, C3DVector(-2.7f, -1.9f, 0.0f), 2.5f);    // truncation toward zero: x -2, y -1, r 2
    if (g_LastBox.TLX != -4 || g_LastBox.TLY != -3 || g_LastBox.BRX != 0 || g_LastBox.BRY != 1) ++fails;
    if (f.Window.SmashCount != 1 || a.Window.SmashCount != 1) ++fails;
    SmashAllWindowsWithinRadiusOfPoint(world, C3DVector(10.0f, 10.0f, 0.0f), 0.9f);    // r 0: empty box, nothing smashed
    if (a.Window.SmashCount != 1) ++fails;
    if (fails) { std::printf("008a8010_TEST FAIL (%d)\n", fails); return 1; }
    std::printf("008a8010_TEST PASS\n");
    return 0;
}