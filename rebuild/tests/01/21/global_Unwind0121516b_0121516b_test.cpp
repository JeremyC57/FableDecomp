#include <cstdio>
#include <new>
#include <cstdlib>
static int g_ca = 0, g_cb = 0, g_da = 0, g_db = 0, g_oa = 0, g_ob = 0; static bool g_faila = false, g_failb = false; static bool g_opena = true, g_openb = true; static void* g_arg = 0; static void* g_argb = 0;
struct Ds { unsigned char pad[0x10]; void* a; void* b; };
struct Ga { virtual ~Ga() { ++g_da; } virtual bool Open(void* a, void* b, long c) { ++g_oa; g_arg = a; return g_opena; } unsigned char x[0x624]; Ga() { ++g_ca; } static void* operator new(size_t n) throw() { return g_faila ? 0 : std::malloc(n); } static void operator delete(void* p) { std::free(p); } };
struct Gb { virtual ~Gb() { ++g_db; } virtual bool Open(void* a, void* b, long c) { ++g_ob; g_argb = b; return g_openb; } unsigned char x[0x624]; Gb() { ++g_cb; } static void* operator new(size_t n) throw() { return g_failb ? 0 : std::malloc(n); } static void operator delete(void* p) { std::free(p); } };
struct Ow { unsigned char pad[0x180]; Ga* g; Gb* h; unsigned char pad2[0x4c]; long ctx; bool Init(const Ds* d) { g = new Ga; if (!g) return false; if (!g->Open(d->a, d->b, 0)) { delete g; g = 0; return false; } if (ctx != 0) { h = new Gb; if (!h) { delete g; g = 0; return false; } if (!h->Open(d->a, d->b, 0)) { delete g; g = 0; delete h; h = 0; return false; } } return true; } };
static bool Check() { Ow* o = new Ow; Ds d; int a, b; d.a = &a; d.b = &b; o->ctx = 0; o->g = 0; o->h = 0; if (!o->Init(&d) || !o->g || o->h || g_ca != 1 || g_oa != 1 || g_arg != &a) return false; delete o->g; o->ctx = 1; if (!o->Init(&d) || !o->h || g_cb != 1 || g_ob != 1 || g_argb != &b) return false; delete o->g; delete o->h; g_openb = false; if (o->Init(&d) || o->g != 0 || o->h != 0 || g_da != 3 || g_db != 2) return false; g_failb = true; if (o->Init(&d) || o->g != 0 || g_da != 4) return false; g_opena = false; if (o->Init(&d) || o->g != 0 || g_da != 5) return false; g_faila = true; bool ok = !o->Init(&d) && o->g == 0 && g_ca == 5 && g_da == 5; delete o; return ok; }

int main()
{
    if (!Check()) { std::printf("00c3ee10_TEST FAIL\n"); return 1; }
    std::printf("0121516b_TEST PASS\n");
    return 0;
}