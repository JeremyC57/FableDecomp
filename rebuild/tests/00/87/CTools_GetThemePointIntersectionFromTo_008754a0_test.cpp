// Behaviour test for CTools::GetThemePointIntersectionFromTo (008754a0).
#include <cstdio>
struct C3DVector { float x, y, z; };
typedef bool (*TTest)(int);
static bool TestA(int) { return true; }
static bool TestB(int) { return false; }
static float g_eps = 0.0001f;
static TTest g_used = 0; static bool g_flagSeen = true;
static bool GetIntersectionWithWorldMap(const C3DVector* from, const C3DVector* to, C3DVector* pos, C3DVector* normal, bool flag, TTest test)
{
    g_used = test; g_flagSeen = flag;
    pos->x = (from->x + to->x) / 2; pos->y = (from->y + to->y) / 2; pos->z = 7;
    normal->x = 0; normal->y = 0; normal->z = 1;
    return true;
}
static bool GetThemePointIntersectionFromTo(const C3DVector& from, const C3DVector& to, C3DVector& hitPos, C3DVector& hitNormal, bool flag)
{
    float dx = to.x - from.x; float dy = to.y - from.y;
    float d = dx * dx; d += dy * dy;
    if (d > g_eps * g_eps) {
        C3DVector pos, normal; bool hit;
        if (flag) hit = GetIntersectionWithWorldMap(&from, &to, &pos, &normal, flag, TestA);
        else hit = GetIntersectionWithWorldMap(&from, &to, &pos, &normal, flag, TestB);
        if (hit) { hitPos = pos; hitNormal = normal; return true; }
    }
    return false;
}
int main()
{
    C3DVector a = { 0, 0, 5 }, b = { 2, 4, 0 }, p = { -1, -1, -1 }, n = { -1, -1, -1 };
    bool ok = GetThemePointIntersectionFromTo(a, b, p, n, true) && g_used == TestA && g_flagSeen && p.x == 1 && p.y == 2 && p.z == 7 && n.z == 1;
    ok = ok && GetThemePointIntersectionFromTo(a, b, p, n, false) && g_used == TestB && !g_flagSeen;
    C3DVector c = { 0, 0, 9 }; g_used = 0;
    ok = ok && !GetThemePointIntersectionFromTo(a, c, p, n, true) && g_used == 0;    // vertical segment: no horizontal extent
    if (!ok) { std::printf("008754a0_TEST FAIL\n"); return 1; }
    std::printf("008754a0_TEST PASS\n");
    return 0;
}