#include <cstdio>
static int g_l = 0; struct G {}; static G g_g; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
struct K { long k, id, d; }; struct E { long v; }; static E g_e; static long g_id = 0; static long* g_pd = 0; static K* g_k = 0;
struct X { E* Lk(long id, long* d, void* b, K* k, void* c, void* dd) { g_id = id; g_pd = d; g_k = k; return &g_e; } E* Fr(K* k, void* b, void* c, void* d) { L l(&g_g); return Lk(k->id, &k->d, b, k, c, d); } };
static bool Check() { K k; k.id = 9; k.d = 1; X x; E* e = x.Fr(&k, 0, 0, 0); return e == &g_e && g_id == 9 && g_pd == &k.d && g_k == &k && g_l == 0; }

int main()
{
    if (!Check()) { std::printf("00c12cd0_TEST FAIL\n"); return 1; }
    std::printf("00c12cd0_TEST PASS\n");
    return 0;
}
