#include <cstdio>
#include <new>
static int g_l = 0; struct G {}; static G g_g; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
struct X; static int g_rel = 0, g_det = 0, g_find = 0; struct En { unsigned char p[0x24]; void* key; long bound; long IsBound() { return bound; } void Release(X* i) { ++g_rel; } }; static En g_e[3]; struct Ar { long a; unsigned long c; En* Get(unsigned long i) { return &g_e[i]; } }; struct Ob { void Detach(X* i) { ++g_det; } }; static Ob g_ob; static int g_left = 2; struct Obs { long a; Ob* Find(void* k) { ++g_find; return g_left-- > 0 ? &g_ob : 0; } };
struct X { unsigned char p[0x2c]; Obs obs; unsigned char q[0x2c]; Ar ar; void Unbind(void* k) { L l(&g_g); for (unsigned long i = 0; i < ar.c; ++i) { En* e = ar.Get(i); if (e->IsBound() && e->key == k) e->Release(this); } for (Ob* o = obs.Find(k); o; o = obs.Find(k)) o->Detach(this); } };
static bool Check() { int k, j; g_e[0].key = &k; g_e[0].bound = 1; g_e[1].key = &k; g_e[1].bound = 0; g_e[2].key = &j; g_e[2].bound = 1; X* x = new X; x->ar.c = 3; x->Unbind(&k); bool ok = g_rel == 1 && g_det == 2 && g_find == 3 && g_l == 0; delete x; return ok; }

int main()
{
    if (!Check()) { std::printf("00c12520_TEST FAIL\n"); return 1; }
    std::printf("00c12520_TEST PASS\n");
    return 0;
}