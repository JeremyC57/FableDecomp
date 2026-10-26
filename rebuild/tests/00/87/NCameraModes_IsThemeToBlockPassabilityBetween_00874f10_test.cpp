#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#endif
// Model of NCameraModes::IsThemeToBlockPassabilityBetween(map, a, b, flag) @ 0x00874F10: both positions' passability
// is queried, and the segment is blocked exactly when b is not passable (a's answer does not change the result).
class C3DVector { public: float X, Y, Z; };
class CWorldMap {
public:
    float Wall;                                         // positions with X >= Wall are impassable
    mutable int Queries;
    bool IsPosPassable(const C3DVector& pos) const { ++Queries; return pos.X < Wall; }   // 0x004FCC40
};
bool __fastcall IsThemeToBlockPassabilityBetween(const CWorldMap& map, const C3DVector& a, const C3DVector& b, bool)
{
    bool passableA = map.IsPosPassable(a);
    bool passableB = map.IsPosPassable(b);
    if (passableA) {
        if (!passableB)
            return true;
    }
    else {
        if (!passableB)
            return true;
    }
    return false;
}
int main()
{
    bool ok = true;
    CWorldMap map;
    map.Wall = 10.0f;
    map.Queries = 0;
    C3DVector open = { 1.0f, 0.0f, 0.0f };
    C3DVector wall = { 20.0f, 0.0f, 0.0f };
    if (IsThemeToBlockPassabilityBetween(map, open, open, false)) ok = false;
    if (!IsThemeToBlockPassabilityBetween(map, open, wall, false)) ok = false;
    if (IsThemeToBlockPassabilityBetween(map, wall, open, true)) ok = false;
    if (!IsThemeToBlockPassabilityBetween(map, wall, wall, true)) ok = false;
    if (map.Queries != 8) ok = false;                   // both ends are always queried
    if (!ok) { std::printf("00874f10_TEST FAIL\n"); return 1; }
    std::printf("00874f10_TEST PASS\n");
    return 0;
}