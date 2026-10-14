#include <cstdio>
struct HP { float p[3]; long bone; };
struct E { unsigned long first; HP second; };
struct Map { bool c; E* el; unsigned long n; bool d;
    E* LB(const unsigned long& k) { E* p = el; while (p != el + n && p->first < k) ++p; return p; }
    E* End() { return el + n; } };
struct M { virtual void v00() {} unsigned char pad[0x34]; Map hp;
    HP* Get(unsigned long k, unsigned long i) { E* it = hp.LB(k); if (it == hp.End()) return 0; if (it->first != k) return 0; return &it[i].second; } };
int main()
{
    E es[4] = { { 1 }, { 3 }, { 3 }, { 7 } }; M m; m.hp.el = es; m.hp.n = 4;
    if (m.Get(3, 1) != &es[2].second || m.Get(3, 0) != &es[1].second || m.Get(2, 0) != 0 || m.Get(9, 0) != 0) { std::printf("00a2e8c0_TEST FAIL\n"); return 1; }
    std::printf("00a2e8c0_TEST PASS\n");
    return 0;
}
