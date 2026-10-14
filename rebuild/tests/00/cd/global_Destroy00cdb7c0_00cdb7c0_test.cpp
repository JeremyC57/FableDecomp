#include <cstdio>
static int g_d = 0; struct C { virtual ~C() { ++g_d; } unsigned char b[48]; };
static void __fastcall F(C* f, C* l) { for (; f != l; ++f) f->~C(); }
int main()
{
    { C buf[2]; F(buf, buf + 2); if (g_d != 2) { std::printf("00cdb7c0_TEST FAIL\n"); return 1; }
    }

    std::printf("00cdb7c0_TEST PASS\n");
    return 0;
}