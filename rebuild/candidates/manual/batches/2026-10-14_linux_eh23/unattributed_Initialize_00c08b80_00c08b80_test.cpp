#include <cstdio>
#include <new>
static int g_l = 0; struct G {}; static G g_g; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
struct Rec { unsigned char p[0xd0]; void* pl; }; struct Host; static int g_ctor = 0; static void* g_arg = 0; static void* g_tbl = 0; static Host* g_host = 0;
struct Gm { long m0, m4; Rec* rec; unsigned char p[0x98]; Gm(Host* h, void** pl, void* t) { ++g_ctor; g_host = h; g_arg = pl; g_tbl = t; rec = 0; } };
struct Host { unsigned char p[0x64]; Gm* gm; unsigned char q[8]; void* tbl; void Init(void** pl) { L l(&g_g); if (gm) { gm->rec->pl = *pl; return; } if (*pl) gm = new Gm(this, pl, tbl); } };
static bool Check() { Host* h = new Host; int t, p; h->gm = 0; h->tbl = &t; void* pl = 0; h->Init(&pl); if (h->gm || g_ctor) return false; pl = &p; h->Init(&pl); if (!h->gm || g_ctor != 1 || g_host != h || g_arg != &pl || g_tbl != &t) return false; Rec r; r.pl = 0; h->gm->rec = &r; h->Init(&pl); bool ok = r.pl == &p && g_ctor == 1 && g_l == 0; delete h->gm; delete h; return ok; }

int main()
{
    if (!Check()) { std::printf("00c08b80_TEST FAIL\n"); return 1; }
    std::printf("00c08b80_TEST PASS\n");
    return 0;
}
