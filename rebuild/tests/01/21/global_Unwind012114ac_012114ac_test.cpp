#include <cstdio>
#include <new>
static int g_l = 0; struct G {}; static G g_g; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
struct V { float x, y, z; bool Equals(const V& o) const { return x == o.x && y == o.y && z == o.z; } }; static int g_up = 0; static float g_sum = 0; struct Ow { void Update(const V& a, const V& b, const V& c) { ++g_up; g_sum = a.x + b.y + c.z; } };
struct X { unsigned char p[8]; V a, b, c; unsigned char q[0x790]; Ow* o; unsigned char r[8]; unsigned long fl; void Set(const V& A, const V& B, const V& C) { L l(&g_g); if (!(a.Equals(A) && b.Equals(B) && c.Equals(C))) { a = A; b = B; c = C; o->Update(a, b, c); fl |= 4; } } };
static bool Check() { Ow o; X* x = new X; x->o = &o; x->fl = 0; V a = { 1, 2, 3 }, b = { 4, 5, 6 }, c = { 7, 8, 9 }; x->a = a; x->b = b; x->c = c; x->Set(a, b, c); if (g_up) return false; c.z = 10; x->Set(a, b, c); bool ok = g_up == 1 && g_sum == 16 && x->c.z == 10 && x->fl == 4 && g_l == 0; delete x; return ok; }

int main()
{
    if (!Check()) { std::printf("00c10e50_TEST FAIL\n"); return 1; }
    std::printf("00c10e50_TEST PASS\n");
    return 0;
}