#include <cstdio>
struct N { N* next; }; static N* __cdecl S(N* n) { return n->next; } struct I { N* n; };
static long __cdecl F(const I& a, const I& b) { N* n = a.n; N* e = b.n; long c = 0; while (n != e) { n = S(n); ++c; } return c; }
int main()
{
    N n[3]; n[0].next = &n[1]; n[1].next = &n[2]; n[2].next = 0; I a; a.n = &n[0]; I b; b.n = &n[2]; if (F(a, b) != 2 || F(b, b) != 0) { std::printf("005b155a_TEST FAIL\n"); return 1; }

    std::printf("005b155a_TEST PASS\n");
    return 0;
}
