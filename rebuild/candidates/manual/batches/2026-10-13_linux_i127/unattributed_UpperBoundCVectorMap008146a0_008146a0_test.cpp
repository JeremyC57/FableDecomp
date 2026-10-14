#include <cstdio>
struct E { long k; unsigned char p[8]; };
struct M { E* b; E* e; E* U(const long& k) { E* f = b; long n = e - b; while (0 < n) { long h = n / 2; E* m = f + h; if (!(k < m->k)) { f = ++m; n -= h + 1; } else n = h; } return f; } };
int main()
{
    E v[4]; v[0].k = 1; v[1].k = 3; v[2].k = 3; v[3].k = 7; M m; m.b = v; m.e = v + 4; long k = 3; if (m.U(k) != v + 3) { std::printf("008146a0_TEST FAIL\n"); return 1; }
    k = 0; if (m.U(k) != v) { std::printf("008146a0_TEST FAIL\n"); return 1; }

    std::printf("008146a0_TEST PASS\n");
    return 0;
}
