#include <cstdio>
#include <new>
static int g_l = 0; struct G {}; static G g_g; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
struct V { float x, y, z; V(const V& o) : x(o.x), y(o.y), z(o.z) {} }; struct X; static long g_id = -1; static float g_z = 0; static void* g_e = 0; static int g_r; struct Fa { void* Create(X* c, long id, void* a, void* b, const V* p, void* d, void* e) { g_id = id; g_z = p->z; g_e = e; return &g_r; } }; struct St { unsigned char p[0x88c]; long next; }; struct W { unsigned char p[0x38]; St* st; }; struct Ow { unsigned char p[4]; W* w; unsigned char q[4]; Fa* fa; };
struct X { unsigned char p[4]; Ow* o; void* Cr(void* a, void* b, const V* p, void* d, void* e) { L l(&g_g); V loc(*p); long id = o->w->st->next++; return o->fa->Create(this, id, a, b, &loc, d, e); } };
static bool Check() { Fa fa; St* st = new St; st->next = 5; W w; w.st = st; Ow o; o.w = &w; o.fa = &fa; X x; x.o = &o; int a, e; V v(*(V*)&a); v.x = 1; v.y = 2; v.z = 3; bool ok = x.Cr(&a, 0, &v, 0, &e) == &g_r && g_id == 5 && st->next == 6 && g_z == 3 && g_e == &e && g_l == 0; delete st; return ok; }

int main()
{
    if (!Check()) { std::printf("00c2cad0_TEST FAIL\n"); return 1; }
    std::printf("00c2cad0_TEST PASS\n");
    return 0;
}