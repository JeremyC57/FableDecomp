#include <cstdio>
#include <new>
static int g_cn = 0, g_ra = 0, g_rb = 0, g_rc = 0, g_el = 0, g_p94 = 0, g_p144 = 0, g_ho = 0, g_bs = 0;
struct Cn { virtual ~Cn() {} Cn() { ++g_cn; } }; struct Ra : Cn { unsigned char b[0x10]; Ra() { ++g_ra; } }; struct Rb : Cn { unsigned char b[0x14]; Rb() { ++g_rb; } }; struct Rc : Cn { unsigned char b[0x10]; Rc() { ++g_rc; } };
struct El { unsigned char b[0x14]; El() { ++g_el; } ~El() {} }; struct P94 { unsigned char b[0xb0]; P94() { ++g_p94; } ~P94() {} }; struct P144 { unsigned char b[0x9c]; P144() { ++g_p144; } ~P144() {} }; struct Ho { unsigned char b[0x24]; Ho() { ++g_ho; } ~Ho() {} };
struct SbB { virtual void v0() {} long ch, rate, c, d, e, f; unsigned char rest[0x44 - 0x1c]; SbB() { rate = 48000; ch = 2; c = 0; d = 0; e = 0; f = 0; } ~SbB() {} }; struct Sb : SbB { Sb() {} };
struct Bs { virtual ~Bs() {} void* blk; long m8, mc, m10; unsigned char m14; unsigned char rest[0x28 - 0x15]; Bs() : blk(0), m8(0), mc(0), m10(0), m14(0) { ++g_bs; } };
struct K : Bs { Sb m28; Ra m6c, m80; P94 m94; P144 m144; Rb m1e0; El m1f8[4]; Rc m248; Ho m25c; void* buf; long a, b2; unsigned char c; long d; unsigned char e; K() : Bs(), m28(), m6c(), m80(), m94(), m144(), m1e0(), m248(), m25c(), buf(0), a(0), c(0), e(0) { m10 = 0x234; } };
static bool Check() { K* k = new K; bool ok = k->m28.ch == 2 && k->m28.rate == 48000 && k->m28.f == 0 && k->m10 == 0x234 && k->buf == 0 && k->a == 0 && k->c == 0 && k->e == 0 && g_bs == 1 && g_cn == 4 && g_ra == 2 && g_rb == 1 && g_rc == 1 && g_el == 4 && g_p94 == 1 && g_p144 == 1 && g_ho == 1; delete k; return ok; }

int main()
{
    if (!Check()) { std::printf("00c6a650_TEST FAIL\n"); return 1; }
    std::printf("00c6a650_TEST PASS\n");
    return 0;
}
