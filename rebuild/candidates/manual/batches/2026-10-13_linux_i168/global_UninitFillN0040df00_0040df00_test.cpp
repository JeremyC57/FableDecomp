#include <cstdio>
#include <new>
struct E { long v[7]; }; struct A { };
static E* __fastcall F(E* f, unsigned long n, const E& v, A) { for (; 0 < n; --n, ++f) new (f) E(v); return f; }
int main()
{
    E v; for (int i = 0; i < 7; ++i) v.v[i] = i + 1; E buf[3]; E* r = F(buf, 3, v, A()); if (r != buf + 3 || buf[2].v[0] != 1 || buf[1].v[6] != 7) { std::printf("0040df00_TEST FAIL\n"); return 1; }

    std::printf("0040df00_TEST PASS\n");
    return 0;
}
