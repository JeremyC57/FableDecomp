#include <cstdio>
#include <new>
static int g_l = 0; struct G {}; static G g_g; static G g_g2; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
struct X; static X* g_o = 0; static void* g_t = 0; static long g_id = 0; static int g_n = 0; struct Ac { unsigned char b[0x18]; Ac(X* l, void* t, long id) { g_o = l; g_t = t; g_id = id; ++g_n; } };
struct X { unsigned char p[0x60]; Ac* ia; void Add(void* t) { L l(&g_g); if (!ia) ia = new Ac(this, t, 0xAC44); } };
static bool Check() { X* x = new X; x->ia = 0; int a; x->Add(&a); x->Add(&a); bool ok = g_n == 1 && g_o == x && g_t == &a && g_id == 0xAC44 && x->ia != 0 && g_l == 0; delete x->ia; delete x; return ok; }

int main()
{
    if (!Check()) { std::printf("00c08c50_TEST FAIL\n"); return 1; }
    std::printf("00c08c50_TEST PASS\n");
    return 0;
}
