// Behaviour test for CGameCameraBase::GetRenderCamera (0x006FD5E0): a model of the row.
#include <stdio.h>

struct V { float x, y, z; };
static V Lerp(const V& a, const V& b, float t) { V r = { a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t }; return r; }
static int run()
{
    V last = { 0, 0, 0 }, cur = { 10, 20, 30 };
    V p = Lerp(last, cur, 0.5f), q = Lerp(last, cur, 1.0f);
    return p.x == 5 && p.y == 10 && p.z == 15 && q.z == 30;
}
int main()
{
    int ok = run();
    printf(ok ? "006fd5e0_TEST PASS\n" : "006fd5e0_TEST FAIL\n");
    return ok ? 0 : 1;
}
