// Behaviour test for 0x008754A0 NCameraModes::CTools::GetThemePointIntersectionFromTo: links against the source object;
// the private intersection helper, the two theme tests and the epsilon are defined here so the chosen test, the flag
// and the copied hit are observable.
#include <cstdio>
class C3DVector { public: float x, y, z; };
class CWorldMap { public: int Dummy; };
float g_fIntersectionEpsilon = 0.0001f;
namespace NCameraModes {
typedef bool (__fastcall* TBlockTest)(const CWorldMap& map, const C3DVector& from, const C3DVector& to, bool flag);
bool __fastcall IsThemeToBlockCameraBetween(const CWorldMap&, const C3DVector&, const C3DVector&, bool) { return true; }
bool __fastcall IsThemeToBlockPassabilityBetween(const CWorldMap&, const C3DVector&, const C3DVector&, bool) { return false; }
class CTools {
public:
    bool GetThemePointIntersectionFromTo(const C3DVector& from, const C3DVector& to, C3DVector& hitPos, C3DVector& hitNormal,
                                         bool useCameraBlocking) const;
private:
    bool GetIntersectionWithWorldMap(const C3DVector& from, const C3DVector& to, C3DVector& pos, C3DVector& normal, bool flag,
                                     TBlockTest test) const;
};
static TBlockTest g_used = 0; static bool g_flagSeen = true; static bool g_hit = true;
bool CTools::GetIntersectionWithWorldMap(const C3DVector& from, const C3DVector& to, C3DVector& pos, C3DVector& normal, bool flag,
                                         TBlockTest test) const
{
    g_used = test; g_flagSeen = flag;
    pos.x = (from.x + to.x) / 2; pos.y = (from.y + to.y) / 2; pos.z = 7;
    normal.x = 0; normal.y = 0; normal.z = 1;
    return g_hit;
}
}
int main()
{
    using namespace NCameraModes;
    CTools tools;
    C3DVector a = { 0, 0, 5 }, b = { 2, 4, 0 }, p = { -1, -1, -1 }, n = { -1, -1, -1 };
    bool ok = tools.GetThemePointIntersectionFromTo(a, b, p, n, true) && g_used == IsThemeToBlockCameraBetween && g_flagSeen
              && p.x == 1 && p.y == 2 && p.z == 7 && n.z == 1;
    ok = ok && tools.GetThemePointIntersectionFromTo(a, b, p, n, false) && g_used == IsThemeToBlockPassabilityBetween && !g_flagSeen;
    C3DVector c = { 0, 0, 9 }; g_used = 0;
    ok = ok && !tools.GetThemePointIntersectionFromTo(a, c, p, n, true) && g_used == 0;    // vertical segment: no horizontal extent
    g_hit = false; p.x = -5;
    ok = ok && !tools.GetThemePointIntersectionFromTo(a, b, p, n, true) && p.x == -5;      // no hit: outputs untouched
    if (!ok) { std::printf("008754a0_TEST FAIL\n"); return 1; }
    std::printf("008754a0_TEST PASS\n");
    return 0;
}
