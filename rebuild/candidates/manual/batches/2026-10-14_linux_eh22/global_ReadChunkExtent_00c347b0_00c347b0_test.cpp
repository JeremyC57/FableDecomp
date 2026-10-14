#include <cstdio>
#include <new>
static int g_open = 0, g_close = 0; static bool g_ok = true; struct Wf { virtual void v0() {} long off; long m8; long mc; unsigned char p[0x20]; long m30; unsigned char q[0xc]; long m40; unsigned char r[4]; Wf() : off(0), mc(0) {} ~Wf() { Close(); } bool Open(void* a, void* b) { ++g_open; m40 = 100; off = 7; m8 = 20; m30 = 50; return g_ok; } void Close() { ++g_close; } }; struct Ch : Wf { ~Ch() {} };
static long F(void* a, void* b, long* s, long* e) { Ch c; if (!c.Open(a, b)) return -1; *s = c.off + (c.m40 - (long)a); *e = c.m30 - c.m8 + c.m40; return 0; }
static bool Check() { long s = 0, e = 0; g_ok = false; if (F((void*)10, 0, &s, &e) != -1 || g_close != 1) return false; g_ok = true; return F((void*)10, 0, &s, &e) == 0 && s == 97 && e == 130 && g_open == 2 && g_close == 2; }

int main()
{
    if (!Check()) { std::printf("00c347b0_TEST FAIL\n"); return 1; }
    std::printf("00c347b0_TEST PASS\n");
    return 0;
}
