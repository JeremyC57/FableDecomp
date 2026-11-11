#include <cstdio>
#include <new>
struct E { long v[4]; }; struct A { };
static E* __fastcall F(E* f, unsigned long n, const E& v, A) { for (; 0 < n; --n, ++f) new (f) E(v); return f; }
int main()
{
    E v; for (int i = 0; i < 4; ++i) v.v[i] = i + 1; E buf[3]; E* r = F(buf, 3, v, A()); if (r != buf + 3 || buf[2].v[0] != 1 || buf[1].v[3] != 4) { std::printf("00a134d0_TEST FAIL\n"); return 1; }

    std::printf("00a134d0_TEST PASS\n");
    return 0;
}