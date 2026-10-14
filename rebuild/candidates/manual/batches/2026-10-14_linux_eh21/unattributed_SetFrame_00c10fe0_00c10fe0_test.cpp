#include <cstdio>
#include <new>
static int g_l = 0; struct G {}; static G g_g; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
struct V { float x, y, z; V(float X, float Y, float Z) : x(X), y(Y), z(Z) {} }; static float g_s = 0; static int g_up = 0;
struct X { void Update(const V& a, const V& b, const V& c) { ++g_up; g_s = a.x + b.y + c.z; } void SetFrame(const V* a, const V* b, const V* c) { L l(&g_g); V f(a->x, a->y, a->z); V s(b->x, b->y, b->z); V t(c->x, c->y, c->z); Update(f, s, t); } };
static bool Check() { X x; V a(1, 0, 0), b(0, 2, 0), c(0, 0, 4); x.SetFrame(&a, &b, &c); return g_up == 1 && g_s == 7 && g_l == 0; }

int main()
{
    if (!Check()) { std::printf("00c10fe0_TEST FAIL\n"); return 1; }
    std::printf("00c10fe0_TEST PASS\n");
    return 0;
}
