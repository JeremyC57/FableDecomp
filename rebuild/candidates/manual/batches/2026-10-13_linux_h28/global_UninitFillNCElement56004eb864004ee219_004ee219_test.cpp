#include <cstdio>
#include <new>
static int g_c = 0; struct E { unsigned char b[4]; E() {} E(const E& o) { ++g_c; b[0] = o.b[0]; } }; struct A {};
static E* __fastcall UF(E* f, unsigned long n, const E& v) { for (; 0 < n; --n, ++f) new (f) E(v); return f; }
int main()
{
    E v; v.b[0] = 7; unsigned char buf[12]; E* e = UF((E*)buf, 3, v); if (e != (E*)(buf + 12) || g_c != 3 || ((E*)buf)[2].b[0] != 7) { std::printf("004ee219_TEST FAIL\n"); return 1; }

    std::printf("004ee219_TEST PASS\n");
    return 0;
}
