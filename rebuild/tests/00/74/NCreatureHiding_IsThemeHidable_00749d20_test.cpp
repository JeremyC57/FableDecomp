// Behaviour test for 0x00749D20 NCreatureHiding::IsThemeHidable(const CWorldMap&, const C3DVector&)
#include <cstdio>

class C3DVector { public: float X, Y, Z; };
class C2DCoordI { public:
    int X, Y;
    C2DCoordI(int x, int y) : X(x), Y(y) { }
};

static int g_lastX = -999, g_lastY = -999;
class CWorldMap { public:
    // stand-in for 0x004FCBD0: a 4x4 map whose cells (1,2) and (3,0) are blocked; off-map is impassable
    bool IsPosPassable(const C2DCoordI& pos) const
    {
        g_lastX = pos.X; g_lastY = pos.Y;
        if (pos.X < 0 || pos.Y < 0 || pos.X > 3 || pos.Y > 3) return false;
        if ((pos.X == 1 && pos.Y == 2) || (pos.X == 3 && pos.Y == 0)) return false;
        return true;
    }
};

bool IsThemeHidable(const CWorldMap& map, const C3DVector& pos)
{
    return !map.IsPosPassable(C2DCoordI((int)pos.X, (int)pos.Y));
}

int main()
{
    CWorldMap map;
    int ok = 1;
    C3DVector p1 = { 1.9f, 2.7f, 5.0f };                  // truncates to (1,2): blocked -> hidable
    if (!IsThemeHidable(map, p1) || g_lastX != 1 || g_lastY != 2) ok = 0;
    C3DVector p2 = { 2.2f, 2.9f, 0.0f };                  // (2,2): passable -> not hidable
    if (IsThemeHidable(map, p2) || g_lastX != 2 || g_lastY != 2) ok = 0;
    C3DVector p3 = { 3.5f, 0.1f, 0.0f };                  // (3,0): blocked
    if (!IsThemeHidable(map, p3)) ok = 0;
    C3DVector p4 = { -0.5f, 1.0f, 0.0f };                 // truncation toward zero: (0,1) passable
    if (IsThemeHidable(map, p4) || g_lastX != 0) ok = 0;
    if (!ok) { std::printf("00749d20_TEST FAIL\n"); return 1; }
    std::printf("00749d20_TEST PASS\n");
    return 0;
}