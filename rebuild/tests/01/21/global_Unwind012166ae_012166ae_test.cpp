#include <cstdio>
#include <new>
#include <cstdlib>
static int g_free = 0, g_cn = 0, g_ra = 0, g_rb = 0, g_rc = 0, g_el = 0, g_p94 = 0, g_p144 = 0, g_clear = 0, g_ho = 0, g_pl = 0, g_bs = 0, g_rel = 0;
static void Fr(void* p) { ++g_free; std::free(p); }
struct Cn { virtual ~Cn() { ++g_cn; } }; struct Ra : Cn { unsigned char b[0x10]; ~Ra() { ++g_ra; } }; struct Rb : Cn { unsigned char b[0x14]; ~Rb() { ++g_rb; } }; struct Rc : Cn { unsigned char b[0x10]; ~Rc() { ++g_rc; } };
struct El { unsigned char b[0x14]; ~El() { ++g_el; } }; struct P94 { unsigned char b[0xb0]; ~P94() { ++g_p94; } }; struct P144 { unsigned char b[0x9c]; ~P144() { ++g_p144; } }; struct Ho { unsigned char b[0x24]; void Clear() { ++g_clear; } ~Ho() { ++g_ho; } }; struct Pl28 { virtual void v0() {} unsigned char b[0x40]; ~Pl28() { ++g_pl; } };
struct Bs { virtual ~Bs() { ++g_bs; } unsigned char b[0x24]; void ReleaseBlock() { ++g_rel; } };
struct K : Bs { Pl28 m28; Ra m6c, m80; P94 m94; P144 m144; Rb m1e0; El m1f8[4]; Rc m248; Ho m25c; void* blk; long a, b2, c, d; unsigned char f; ~K() { f = 0; if (blk) { Fr(blk); blk = 0; } ReleaseBlock(); m25c.Clear(); } };
static bool Check() { K* k = new K; k->blk = std::malloc(4); k->f = 1; delete k; K* j = new K; j->blk = 0; delete j; return g_free == 1 && g_rel == 2 && g_clear == 2 && g_ho == 2 && g_rc == 2 && g_el == 8 && g_rb == 2 && g_p144 == 2 && g_p94 == 2 && g_ra == 4 && g_pl == 2 && g_bs == 2 && g_cn == 8; }

int main()
{
    if (!Check()) { std::printf("00c6a780_TEST FAIL\n"); return 1; }
    std::printf("012166ae_TEST PASS\n");
    return 0;
}