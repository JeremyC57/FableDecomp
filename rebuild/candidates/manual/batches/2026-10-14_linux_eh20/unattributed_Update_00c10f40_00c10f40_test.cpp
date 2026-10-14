#include <cstdio>
#include <new>
static int g_l = 0; struct G {}; static G g_g; static G g_g2; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
static int g_up = 0; struct V { float x, y, z; V() {} V(const V& o) : x(o.x), y(o.y), z(o.z) {} }; static bool Eq(const V* a, const V* b) { return a->x == b->x && a->y == b->y && a->z == b->z; } struct Ow { void Update(const V* a, const V* b, const V* c) { ++g_up; } };
struct X { unsigned char p[8]; V pos; V m14; V m20; unsigned char q[0x790]; Ow* o; unsigned char r[8]; unsigned long fl; void U(const V& v) { L l(&g_g); if (!Eq(&pos, &v)) { pos = v; o->Update(&pos, &m14, &m20); fl |= 4; } } };
static bool Check() { Ow o; X* x = new X; x->o = &o; x->fl = 0; x->pos.x = 1; x->pos.y = 2; x->pos.z = 3; V v; v.x = 1; v.y = 2; v.z = 3; x->U(v); if (g_up != 0) return false; v.z = 4; x->U(v); bool ok = g_up == 1 && x->pos.z == 4 && x->fl == 4 && g_l == 0; delete x; return ok; }

int main()
{
    if (!Check()) { std::printf("00c10f40_TEST FAIL\n"); return 1; }
    std::printf("00c10f40_TEST PASS\n");
    return 0;
}
