// Behaviour test for 0x00874EE0: IsThemeToBlockCameraBetween is true exactly when the end position is not
// camera-passable (both ends are queried).
#include <cstdio>

class C3DVector { public: float X, Y, Z; };

class CWorldMap
{
public:
    mutable int queries;
    bool IsPosCameraPassable(const C3DVector& pos) const { ++queries; return pos.X < 100.0f; }
};

bool IsThemeToBlockCameraBetween(const CWorldMap& map, const C3DVector& from, const C3DVector& to, bool flag)
{
    bool passableFrom = map.IsPosCameraPassable(from);
    bool passableTo = map.IsPosCameraPassable(to);
    if (passableFrom)
    {
        if (!passableTo)
            return true;
    }
    else
    {
        if (!passableTo)
            return true;
    }
    return false;
}

int main()
{
    CWorldMap map; map.queries = 0;
    C3DVector open; open.X = 1; open.Y = 0; open.Z = 0;
    C3DVector wall; wall.X = 200; wall.Y = 0; wall.Z = 0;
    int fails = 0;
    if (IsThemeToBlockCameraBetween(map, open, open, false)) ++fails;
    if (!IsThemeToBlockCameraBetween(map, open, wall, true)) ++fails;
    if (IsThemeToBlockCameraBetween(map, wall, open, false)) ++fails;
    if (!IsThemeToBlockCameraBetween(map, wall, wall, false)) ++fails;
    if (map.queries != 8) ++fails;
    if (fails) { std::printf("00874ee0_TEST FAIL\n"); return 1; }
    std::printf("00874ee0_TEST PASS\n");
    return 0;
}