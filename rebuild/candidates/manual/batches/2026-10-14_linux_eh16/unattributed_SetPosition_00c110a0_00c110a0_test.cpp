#include <cstdio>
static int g_l = 0; struct G {}; static G g_g; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
struct V { float x, y, z; V(float X, float Y, float Z) : x(X), y(Y), z(Z) {} }; static float g_s = 0;
struct X { void U(const V& v) { g_s = v.x + 2 * v.y + 4 * v.z; } void Set(const V* p) { L l(&g_g); V local(p->x, p->y, p->z); U(local); } };
static bool Check() { V v(1.0f, 2.0f, 3.0f); X x; x.Set(&v); return g_s == 17.0f && g_l == 0; }

int main()
{
    if (!Check()) { std::printf("00c110a0_TEST FAIL\n"); return 1; }
    std::printf("00c110a0_TEST PASS\n");
    return 0;
}
